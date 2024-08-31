#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QByteArray>
#include <QHostAddress>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Convert the QByteArray to QHostAddress IPv4
 * @param host Host address
 * @return QHostAddress
 */
QHostAddress toIPV4QHostAddress(const QByteArray& host);

/**
 * @brief Convert the QByteArray to QHostAddress IPv6
 * @param host Host address
 * @return QHostAddress
 */
QHostAddress toIPV6QHostAddress(const QByteArray& host);

/**
 * @brief Convert the QHostAddress to QByteArray IPv4
 * @param host Host address
 * @return QByteArray
 */
QByteArray toIPV4QByteArray(const QHostAddress& host);

/**
 * @brief Convert the QHostAddress to QByteArray IPv6
 * @param host Host address
 * @return QByteArray
 */
QByteArray toIPV6QByteArray(const QHostAddress& host);
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
