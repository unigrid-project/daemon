// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/filesystem.hpp>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <generatecert.h>
#include <iostream>
#include <util.h>


#include <openssl/pem.h>
#include <openssl/x509.h>

/* Generates a 2048-bit RSA key. */
EVP_PKEY* generate_key()
{
    LogPrintf("Attempting to allocate memory for the EVP_PKEY structure. \n");
    /* Allocate memory for the EVP_PKEY structure. */
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (!pkey) {
        LogPrintf("Unable to create EVP_PKEY structure. \n");
        return NULL;
    }

    /* Generate the RSA key and assign it to pkey. */
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!EVP_PKEY_assign_RSA(pkey, rsa)) {
        LogPrintf("Unable to generate 2048-bit RSA key. \n");
        EVP_PKEY_free(pkey);
        return NULL;
    }

    /* The key has been generated, return it. */
    return pkey;
}

/* Generates a self-signed x509 certificate. */
X509* generate_x509(EVP_PKEY* pkey)
{
    /* Allocate memory for the X509 structure. */
    X509* x509 = X509_new();
    if (!x509) {
        LogPrintf("Unable to create X509 structure. \n");
        return NULL;
    }

    /* Set the serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    /* This certificate is valid from now until exactly one year from now. */
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

    /* Set the public key for our certificate. */
    X509_set_pubkey(x509, pkey);

    /* We want to copy the subject name to the issuer name. */
    X509_NAME* name = X509_get_subject_name(x509);

    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"SE", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"UNIGRID", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"localhost", -1, -1, 0);

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

bool write_to_disk(EVP_PKEY* pkey, X509* x509)
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
        return true; // Nothing to read, so just return
    }
    /* Open the PEM file for writing the certificate to disk. */
    boost::filesystem::path pathSslCert = GetSslCert();
    boost::filesystem::ifstream streamConfig(pathSslCert);
    if (!streamConfig.good()) {
        FILE* x509_file = fopen(pathSslCert.string().c_str(), "wb");

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
        ValidateCertFiles();
    }
}

void GenerateCert::CreateCertFile()
{
    /* Call generate and save to disk */
    /* Generate the key. */
    LogPrintf("Generating RSA key... \n");
    
    EVP_PKEY * pkey = generate_key();
    LogPrintf("After generate_key().. \n");
    if(!pkey)
        LogPrintf("Error generating key... \n");
    
    /* Generate the certificate. */
    LogPrintf("Generating x509 certificate... \n");
    
    X509 * x509 = generate_x509(pkey);
    if(!x509)
    {
        EVP_PKEY_free(pkey);
        LogPrintf("Error generating x509 certificate... \n");
    }
    
    /* Write the private key and certificate out to disk. */
    LogPrintf("Writing key and certificate to disk... \n");
    
    bool ret = write_to_disk(pkey, x509);
    EVP_PKEY_free(pkey);
    X509_free(x509);
    
    if(ret)
    {
        LogPrintf("Success writing key and certificate to disk... \n");
    }
}


void GenerateCert::ValidateCertFiles()
{
    // Check if files exist
    LogPrintf("Checking if cert files exist \n");
    // Validate the files
    LogPrintf("Validating cert files \n");
}
