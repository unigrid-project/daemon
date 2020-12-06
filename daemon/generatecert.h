// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _GENERATE_CERT_H
#define _GENERATE_CERT_H


#include <boost/filesystem.hpp>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <util.h>

#include <openssl/pem.h>
#include <openssl/x509.h>



class GenerateCert
{
private:
    EVP_PKEY * generate_key(){};
    X509 * generate_x509(EVP_PKEY * pkey){};
    bool write_to_disk(EVP_PKEY * pkey, X509 * x509){};
    void CreateCertFile();
    void ValidateCertFiles();
public:
    GenerateCert() {}
        void setup();
};


#endif //_GENERATE_CERT_H