#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

/// @authors : Dharun RK, Sri Lakshmi Kanthan P
/// @file : sslcert.hpp

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>

// C++ headers
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// openssl headers
#include <openssl/pem.h>
#include <openssl/x509.h>

namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal {
/**
 *  @brief generates a EVP_PEY using RSA_generate_key_ex
 *  @returns EVP_PKEY* - shared pointer to the key
 */
std::shared_ptr<EVP_PKEY> generateRSAKey(int bits = 2048) {
  // create a shared pointer to handle the memory
  std::shared_ptr<EVP_PKEY> pkey(EVP_PKEY_new(), EVP_PKEY_free);

  // if pkey is null then throw an error
  if (pkey.get() == NULL) {
    throw std::runtime_error("Can't Create EVP_PKEY");
  }

  // rsa_key = RSA_new();
  std::shared_ptr<RSA> rsa(RSA_new(), RSA_free);

  // if rsa is null then throw an error
  if (rsa.get() == NULL) {
    throw std::runtime_error("Can't Create RSA");
  }

  // BIGNUM *bn_exp = BN_new();
  std::shared_ptr<BIGNUM> bn_exp(BN_new(), BN_free);

  // if bn_exp is null then throw an error
  if (bn_exp.get() == NULL) {
    throw std::runtime_error("Can't Create BIGNUM");
  }

  // Set the exponent to F4
  if (!BN_set_word(bn_exp.get(), RSA_F4)) {
    throw std::runtime_error("Can't Set BIGNUM");
  }

  // Try to generate the key
  if (!RSA_generate_key_ex(rsa.get(), bits, bn_exp.get(), NULL)) {
    throw std::runtime_error("Can't Generate RSA Key");
  }

  // Set the rsa key to the pkey
  if (!EVP_PKEY_set1_RSA(pkey.get(), rsa.get())) {
    throw std::runtime_error("Can't Assign RSA to EVP_PKEY");
  }

  // The key has been generated, return it
  return pkey;
}

/**
 * @brief Generates a self-signed x509 certificate
 *
 * @param pkey an EVP_PKEY pointer
 *
 * @return X509* - shared pointer to certificate
 */
std::shared_ptr<X509> generateX509(std::shared_ptr<EVP_PKEY> pkey) {
  // Allocate memory for the X509 structure.
  std::shared_ptr<X509> x509(X509_new(), X509_free);

  // if x509 is null then throw an error
  if (x509.get() == NULL) {
    throw std::runtime_error("Can't Create X509");
  }

  // Set the public key for our certificate.
  X509_set_pubkey(x509.get(), pkey.get());

  // Actually sign the certificate with our key.
  if (!X509_sign(x509.get(), pkey.get(), EVP_sha1())) {
    throw std::runtime_error("Can't Sign X509");
  }

  // return the certificate
  return x509;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get the Q Ssl Configuration object
 * @param bits - RSA key size
 * @return QSslConfiguration
 */
QSslConfiguration getQSslConfiguration(int bits = 2048) {
  // Generate the RSA key for the certificate
  std::shared_ptr<EVP_PKEY> pkey = internal::generateRSAKey(bits);

  // Generate the certificate
  std::shared_ptr<X509> x509 = internal::generateX509(pkey);

  // using the defer as in golang
  using Defer = std::shared_ptr<void>;

  // Write the key to buffer
  BIO *pkey_buffer = BIO_new(BIO_s_mem());

  // Free the memory
  Defer defer_pkey_buffer(pkey_buffer, BIO_free_all);

  // Write the key to buffer
  PEM_write_bio_PrivateKey(pkey_buffer, pkey.get(), NULL, NULL, 0, NULL, NULL);

  // Get the key from buffer
  BUF_MEM *pkey_buffer_memory;

  // Get the key from buffer
  BIO_get_mem_ptr(pkey_buffer, &pkey_buffer_memory);

  // QByteArray for the Key
  QByteArray key(QByteArray(pkey_buffer_memory->data, pkey_buffer_memory->length));

  // Create the QSslKey
  QSslKey sslKey(key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);

  // Write the certificate to buffer
  BIO *x509_buffer = BIO_new(BIO_s_mem());

  // Free the memory
  Defer defer_x509_buffer(x509_buffer, BIO_free_all);

  // Write the certificate to buffer
  PEM_write_bio_X509(x509_buffer, x509.get());

  // Get the certificate from buffer
  BUF_MEM *x509_buffer_memory;

  // Get the certificate from buffer
  BIO_get_mem_ptr(x509_buffer, &x509_buffer_memory);

  // QByteArray for the Certificate
  QByteArray cert(QByteArray(x509_buffer_memory->data, x509_buffer_memory->length));

  // Create the QSslCertificate
  QSslCertificate sslCert(cert, QSsl::Pem);

  // Create the QSslConfiguration
  QSslConfiguration sslConfig;

  // Add the certificate to the configuration
  sslConfig.addCaCertificate(sslCert);

  // Add the key to the configuration
  sslConfig.setPrivateKey(sslKey);

  // check if the configuration is valid
  if (sslConfig.isNull()) {
    throw std::runtime_error("Can't Create QSslConfiguration");
  }

  // return the configuration
  return sslConfig;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
