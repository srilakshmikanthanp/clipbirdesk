// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "storage.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
/**
 * @brief Construct a new SQLStore object
 *
 * @param parent
 */
Storage::Storage(QObject *parent = nullptr) : QObject(parent) {
  this->settings->setParent(this);
}

/**
 * @brief Store Client hostname and JWT token
 *
 * @param hostname
 * @param token
 */
void Storage::setClientToken(const QString &hostname, const QString &token) {
  settings->beginGroup(clientGroup);
  settings->setValue(hostname, token);
  settings->endGroup();
}

/**
 * @brief Get the JWT token for the hostname
 *
 * @param hostname
 * @return QString
 *
 * @throw std::invalid_argument if hostname not found
 */
QString Storage::getClientToken(const QString &hostname) {
  settings->beginGroup(clientGroup);
  auto token = settings->value(hostname).toString();
  settings->endGroup();

  if (token.isEmpty()) {
    throw std::invalid_argument("hostname not found");
  }

  return token;
}

/**
 * @brief Store the server hostname and JWT token
 *
 * @param hostname
 * @param token
 */
void Storage::setServerToken(const QString &hostname, const QString &token) {
  settings->beginGroup(serverGroup);
  settings->setValue(hostname, token);
  settings->endGroup();
}

/**
 * @brief Get the JWT token for the hostname
 *
 * @param hostname
 * @return QString
 *
 * @throw std::invalid_argument if hostname not found
 */
QString Storage::getServerToken(const QString &hostname) {
  settings->beginGroup(serverGroup);
  auto token = settings->value(hostname).toString();
  settings->endGroup();

  if (token.isEmpty()) {
    throw std::invalid_argument("hostname not found");
  }

  return token;
}
}  // namespace srilakshmikanthanp::clipbirdesk::storage
