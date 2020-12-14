// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/filesystem.hpp>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <generatecert.h>
#include <iostream>
#include <random>
#include <regex>
#include <util.h>

#include <openssl/pem.h>
#include <openssl/x509.h>

/* Generates a 2048-bit RSA key. */
EVP_PKEY* GenerateCert::generate_key()
{
    LogPrintf("Attempting to allocate memory for the EVP_PKEY structure. \n");
    /* Allocate memory for the EVP_PKEY structure. */
    EVP_PKEY* pkey = EVP_PKEY_new();

    /* Generate a RSA key and assign it to pkey. */
    BIGNUM* bne = BN_new();
    BN_set_word(bne, RSA_F4);
    RSA* rsa = RSA_new();
    RSA_generate_key_ex(rsa, 2048, bne, nullptr);

    EVP_PKEY_assign_RSA(pkey, rsa);
    LogPrintf("Returning new key. \n");
    return pkey;
}

/* Generates a self-signed x509 certificate. */
X509* GenerateCert::generate_x509(EVP_PKEY* pkey)
{
    /* Allocate memory for the X509 structure. */
    X509* x509 = X509_new();
    if (!x509) {
        LogPrintf("Unable to create X509 structure. \n");
        return NULL;
    }

    /* Set the serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    /* This certificate is valid from now until exactly 10 years from now. */
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 315360000L);

    /* Set the public key for our certificate. */
    X509_set_pubkey(x509, pkey);

    /* We want to copy the subject name to the issuer name. */
    X509_NAME* name = X509_get_subject_name(x509);

    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"SE", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"UNIGRID", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)GenerateCert::gen_random(15), -1, -1, 0);
    LogPrintf("random string: %s \n", GenerateCert::gen_random(15));
    /* Now set the issuer name. */
    X509_set_issuer_name(x509, name);

    /* Actually sign the certificate with our key. */
    if (!X509_sign(x509, pkey, EVP_sha1())) {
        LogPrintf("Error signing certificate. \n");
        X509_free(x509);
        return NULL;
    }

    return x509;
}

bool GenerateCert::write_to_disk(EVP_PKEY* pkey, X509* x509)
{
    /* Open the PEM file for writing the key to disk. */
    boost::filesystem::path pathKey = GetSslKey();
    boost::filesystem::ifstream keyStreamConfig(pathKey);
    if (!keyStreamConfig.good()) {
        FILE* pkey_file = fopen(pathKey.string().c_str(), "wb");

        if (!pkey_file) {
            LogPrintf("Unable to open %s for writing. \n", pathKey);
            return false;
        }

        /* Write the key to disk. */
        bool ret = PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(pkey_file);

        if (!ret) {
            LogPrintf("Unable to write private key to disk. \n");
            return false;
        }
    }
    /* Open the PEM file for writing the certificate to disk. */
    boost::filesystem::path pathSslCert = GetSslCert();
    boost::filesystem::ifstream streamConfig(pathSslCert);
    if (!streamConfig.good()) {
        FILE* x509_file = fopen(pathSslCert.string().c_str(), "wb");
        LogPrintf("Attempting to save cert to disk. \n");
        if (!x509_file) {
            LogPrintf("Unable to open %s for writing. \n", pathSslCert);
            return false;
        }

        /* Write the certificate to disk. */
        bool ret = PEM_write_X509(x509_file, x509);
        fclose(x509_file);
        if (!ret) {
            LogPrintf("Unable to write certificate to disk. \n");
            return false;
        }
        return true; // Nothing to read, so just return
    } else {
        LogPrintf("Error opening certificate. \n");
    }

    return true;
}

void GenerateCert::setup()
{
    boost::filesystem::path certDir = GetDataDir() / "certificates";
    if (!boost::filesystem::exists(certDir)) {
        LogPrintf("Certificates folder does not exist %u \n", boost::filesystem::exists(certDir));
        boost::filesystem::create_directories(certDir);
        CreateCertFile();
    } else {
        // Check cert files are valid
        LogPrintf("Certificates folder exists %u \n", boost::filesystem::exists(certDir));
        GenerateCert::ValidateCertFiles();
    }
}

void GenerateCert::CreateCertFile()
{
    /* Call generate and save to disk */
    /* Generate the key. */
    LogPrintf("Generating RSA key... \n");

    EVP_PKEY* pkey = GenerateCert::generate_key();
    LogPrintf("After generate_key().. \n");
    if (!pkey)
        LogPrintf("Error generating key... \n");

    /* Generate the certificate. */
    LogPrintf("Generating x509 certificate... \n");

    X509* x509 = GenerateCert::generate_x509(pkey);
    if (!x509) {
        EVP_PKEY_free(pkey);
        LogPrintf("Error generating x509 certificate... \n");
    }

    /* Write the private key and certificate out to disk. */
    LogPrintf("Writing key and certificate to disk... \n");

    bool ret = GenerateCert::write_to_disk(pkey, x509);
    EVP_PKEY_free(pkey);
    X509_free(x509);

    if (ret) {
        LogPrintf("Success writing key and certificate to disk... \n");
        /* validate the key and cert */
        GenerateCert::ValidateCertFiles();
    }
}


void GenerateCert::ValidateCertFiles()
{
    // Check if files exist
    boost::filesystem::path pathSslCert = GetSslCert();
    boost::filesystem::path pathKey = GetSslKey();

    LogPrintf("Attempting to validate SSL certificates. \n");

    std::string output;
    std::fstream certFile;
    std::fstream keyFile;
    char* cert_pem = new char[3000];
    char* key_pem = new char[3000];
    std::smatch match;
    std::regex rgx_pk("-----BEGIN PRIVATE KEY-----\n([A-Za-z0-9\\/\\+\n]+)");
    std::regex rgx_pbk("-----BEGIN CERTIFICATE-----\n([A-Za-z0-9\\/\\+\n]+)");
    certFile.open(pathSslCert.string().c_str(), std::ios::in);
    if (!certFile.is_open()) {
        LogPrintf("no certFile %\n");
    } else {
        std::string certFileContent((std::istreambuf_iterator<char>(certFile)),
            std::istreambuf_iterator<char>());
        //cert_pem = certFileContent.c_str();
        std::strncpy(cert_pem, certFileContent.c_str(), certFileContent.size());
        // cert_pem[certFileContent.size() + 1];
        LogPrintf("certFileContent.size()... %s \n", certFileContent.size());
        LogPrintf("cert_pem size... %s \n", strlen(cert_pem));


        if (std::regex_search(certFileContent, match, rgx_pbk)) {
            //std::strncpy(cert_pem, match.str(0).c_str(), match.str(0).size());
            LogPrintf("certFile: %s \n", certFileContent);
        } else {
            LogPrintf("no match certFile... %s \n", certFileContent);
        }
    }
    keyFile.open(pathKey.string().c_str(), std::ios::in);
    if (!keyFile.is_open()) {
        LogPrintf("no keyFile %\n");
    } else {
        std::string keyFileContent((std::istreambuf_iterator<char>(keyFile)),
            std::istreambuf_iterator<char>());
        //key_pem = keyFileContent.c_str();
        std::strncpy(key_pem, keyFileContent.c_str(), keyFileContent.size());
        LogPrintf("certFileContent.size()... %s \n", keyFileContent.size());
        LogPrintf("cert_pem size... %s \n", strlen(key_pem));

        if (std::regex_search(keyFileContent, match, rgx_pk)) {
            LogPrintf("keyFile: %s \n", match.str());
        } else {
            LogPrintf("no match keyFile... %s \n", keyFileContent);
        }
    }

    //const char* cert_pem = certFileContent.c_str();
    //const char* key_pem = keyFileContent.c_str();

    //LogPrintf("is test cert valid? %s \n", GenerateCert::sig_verify(cert, intermediate));
    LogPrintf("is cert valid? %s \n", GenerateCert::sig_verify(cert_pem, key_pem));

    keyFile.close();
    certFile.close();


    /*} else {
        LogPrintf("pbkey is not valid and needs to be generated again. \n");
    }*/

    // Validate the files
    LogPrintf("Validating cert files \n");
}

int GenerateCert::sig_verify(const char* cert_pem, const char* key_pem)
{
    LogPrintf("attempting to validate keys  \n");
    BIO* b = BIO_new(BIO_s_mem());
    BIO_puts(b, key_pem);
    X509* issuer = PEM_read_bio_X509(b, NULL, NULL, NULL);
    EVP_PKEY* signing_key = X509_get_pubkey(issuer);
    LogPrintf("after EVP_PKEY \n");
    BIO* c = BIO_new(BIO_s_mem());
    BIO_puts(c, cert_pem);
    X509* x509 = PEM_read_bio_X509(c, NULL, NULL, NULL);

    int result = X509_verify(x509, signing_key);

    EVP_PKEY_free(signing_key);
    BIO_free(b);
    BIO_free(c);
    X509_free(x509);
    X509_free(issuer);

    return result;
}

char* GenerateCert::gen_random(const int len)
{
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        char* rnd_char = new char[15];
    std::strncpy(rnd_char, tmp_s.c_str(), tmp_s.size());
    return rnd_char;
}
