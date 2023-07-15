#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard
// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QFile>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
// C++ headers
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// openssl headers
#include <openssl/pem.h>
#include <openssl/x509.h>


class CertificateInstaller : public QObject {
 public:  //}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
  /**
   *  @brief generates a EVP_PEY
   *
   *  @returns pkey - an EVP_KEY  is returned
   */

  EVP_PKEY *generate_key() {
    /* Allocate memory for the EVP_PKEY structure. */
    EVP_PKEY *pkey = EVP_PKEY_new();
    if (!pkey) {
      std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
      return NULL;
    }

    /* Generate the RSA key and assign it to pkey. */
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!EVP_PKEY_assign_RSA(pkey, rsa)) {
      std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
      EVP_PKEY_free(pkey);
      return NULL;
    }

    /* The key has been generated, return it. */
    return pkey;
  }

  /**
   * @brief Generates a self-signed x509 certificate
   *
   * @param pkey an EVP_PKEY pointer
   *
   * @return bool - true if signing successfull else false
   */
  X509 *generate_x509(EVP_PKEY *pkey) {
    /* Allocate memory for the X509 structure. */
    X509 *x509 = X509_new();
    if (!x509) {
      std::cerr << "Unable to create X509 structure." << std::endl;
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
    X509_NAME *name = X509_get_subject_name(x509);

    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"CA",
                               -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC,
                               (unsigned char *)"ClipBird", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                               (unsigned char *)"localhost", -1, -1, 0);

    /* Now set the issuer name. */
    X509_set_issuer_name(x509, name);

    /* Actually sign the certificate with our key. */
    if (!X509_sign(x509, pkey, EVP_sha1())) {
      std::cerr << "Error signing certificate." << std::endl;
      X509_free(x509);
      return NULL;
    }

    return x509;
  }

  /**
   * @brief Writes the key and certificates to
   * the disk in corresponding names :
   * key.pem and cert.pem
   *
   * @param pkey an EVP_PKEY pointer
   * @param x509 x509 certificate pointer
   *
   * @return bool - true if write success else false
   */

  bool write_to_disk(EVP_PKEY *pkey, X509 *x509) {
    /* Open the PEM file for writing the key to disk. */

    BIO *pkey_file = BIO_new_file("key.pem", "wb");
    if (!pkey_file) {
      std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
      return false;
    }

    /* Write the key to disk. */
    bool ret =
        PEM_write_bio_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
    BIO_free_all(pkey_file);

    if (!ret) {
      std::cerr << "Unable to write private key to disk." << std::endl;
      return false;
    }

    /** Writing certificate to the disk using B i/o since
     *  windows updated the file handling abstractions
     */
    BIO *x509file = BIO_new_file("cert.pem", "wb");
    if (!x509file) {
      return false;
    }
    if (!PEM_write_bio_X509(x509file, x509)) {
      return false;
    }
    BIO_free_all(x509file);

    if (!ret) {
      std::cerr << "Unable to write certificate to disk." << std::endl;
      return false;
    }

    return true;
  }

  /**
   * @brief calls the key and certificate generate fucntion to create a self
   * signed certificate
   *
   * @return X509* - certificate pointer
   */
  X509 *generateCertificate() {
    EVP_PKEY *pkey = generate_key();
    if (!pkey) return NULL;

    /* Generate the certificate. */
    X509 *x509 = generate_x509(pkey);

    if (!x509) {
      EVP_PKEY_free(pkey);
      return NULL;
    }

    /*Writing files to the disk (certificate and key files)"*/
    bool ret = write_to_disk(pkey, x509);

    // EVP_PKEY_free(pkey);  //clears the key from memory

    // X509_free(x509);

    if (ret)
      return x509;
    else
      return NULL;
  }

  /**
   * @brief Generate a self signed certificate and key and return in
   * as QSslConfiguration
   * @param // add as you need
   * @author Dharun R K
   * @return QSslConfiguration
   */
  QSslConfiguration generateSslConfig() {
    X509* x509=generateCertificate();
    QFile file("cert.pem");
 
    if (!file.open(QIODevice::ReadOnly)) {
      qFatal("Could not load certificate!");
    }
     
    const QByteArray bytes = file.readAll();

    const QSslCertificate certificate(bytes,QSsl::Pem);

   
    /// Add our own CA to the default SSL configuration
    QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();


    auto certs = configuration.caCertificates();
   
    certs.append(certificate);
  
    configuration.addCaCertificates(certs);
    QSslConfiguration::setDefaultConfiguration(configuration);
    return configuration;

  }
};

