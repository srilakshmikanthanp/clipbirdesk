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
std::shared_ptr<EVP_PKEY> generateRSAKey(int bits = 2048);

/**
 * @brief Generates a self-signed x509 certificate
 *
 * @param pkey an EVP_PKEY pointer
 *
 * @return X509* - shared pointer to certificate
 */
std::shared_ptr<X509> generateX509(std::shared_ptr<EVP_PKEY> pkey);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get the Q Ssl Configuration object
 * @param bits - RSA key size
 * @return QSslConfiguration
 */
QSslConfiguration getQSslConfiguration(int bits = 2048);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
