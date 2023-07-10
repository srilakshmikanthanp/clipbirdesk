#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>

// C++ headers
#include <string>
#include <vector>

// openssl headers
#include <openssl/pem.h>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Generate a self signed certificate and key and return in
 * as QSslConfiguration
 * @param // add as you need
 * @author Dharun R K
 * @return QSslConfiguration
 */
QSslConfiguration generateSelfSignedConfiguration() {

}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
