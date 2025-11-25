#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

/// @authors : Dharun RK, Sri Lakshmi Kanthan P
/// @file : sslcert.hpp

// Qt headers
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
#include <QHostInfo>

// C++ headers
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// openssl headers
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/err.h>

// local headers
#include "constants/constants.hpp"
#include "common/types/ssl_config/ssl_config.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions::internal {
  /**
   * @brief Get the OpenSSL error as string
   * @return std::string - OpenSSL error
   */
  std::string getOpenSSLError();
}

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Generate a RSA Key Pair and return as QSslKey
 * @param bits - RSA key size
 * @return QPair<QSslKey, QSslKey> - first is private key, second is public key
 */
QPair<QSslKey, QSslKey> generateQtKeyPair(int bits = 2048);

/**
 * @brief Get the Q Ssl Configuration object
 * @param bits - RSA key size
 * @return QSslConfiguration
 */
common::types::SslConfig getQSslConfiguration(int bits = 2048);

/**
 * @brief Generate AES Key
 * @param bits - AES key size
 * @return QByteArray - AES key
 */
QByteArray generateAESKey(int bits = 256);

/**
 * @brief Generate IV
 * @param bits - IV bits
 * @return QByteArray - IV
 */
QByteArray generateIV(int bits = 96);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
