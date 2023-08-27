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
Storage::Storage(QObject *parent) : QObject(parent) {
  this->settings->setParent(this);
}

/**
 * @brief Store Client hostname and JWT token
 *
 * @param hostname
 * @param token
 */
void Storage::setClientCert(const QString &hostname, const QString &token) {
  settings->beginGroup(clientGroup);
  settings->setValue(hostname, token);
  settings->endGroup();
}

/**
 * @brief has the cert for the hostname
 */
bool Storage::hasClientCert(const QString &hostname) {
  settings->beginGroup(clientGroup);
  auto token = settings->value(hostname);
  settings->endGroup();

  return !token.isNull();
}

/**
 * @brief Get the JWT token for the hostname
 *
 * @param hostname
 * @return QString
 *
 * @throw std::invalid_argument if hostname not found
 */
QString Storage::getClientCert(const QString &hostname) {
  settings->beginGroup(clientGroup);
  auto token = settings->value(hostname);
  settings->endGroup();

  if (token.isNull()) {
    throw std::invalid_argument("hostname not found");
  }

  return token.toString();
}

/**
 * @brief Store the server hostname and JWT token
 *
 * @param hostname
 * @param token
 */
void Storage::setServerCert(const QString &hostname, const QString &token) {
  settings->beginGroup(serverGroup);
  settings->setValue(hostname, token);
  settings->endGroup();
}

/**
 * @brief has the cert for the hostname
 */
bool Storage::hasServerCert(const QString &hostname) {
  settings->beginGroup(serverGroup);
  auto token = settings->value(hostname);
  settings->endGroup();

  return !token.isNull();
}

/**
 * @brief Get the JWT token for the hostname
 *
 * @param hostname
 * @return QString
 *
 * @throw std::invalid_argument if hostname not found
 */
QString Storage::getServerCert(const QString &hostname) {
  settings->beginGroup(serverGroup);
  auto token = settings->value(hostname);
  settings->endGroup();

  if (token.isNull()) {
    throw std::invalid_argument("hostname not found");
  }

  return token.toString();
}

/**
 * @brief Set the current state of the server or client
 *
 * @param isServer
 */
void Storage::setHostIsServer(bool isServer) {
  settings->beginGroup(generalGroup);
  settings->setValue(hostStateKey, isServer);
  settings->endGroup();
}

/**
 * @brief Get the current state of the server or client
 *
 * @return true  if server
 * @return false if client
 */
bool Storage::getHostIsServer() {
  settings->beginGroup(generalGroup);
  auto isServer = settings->value(hostStateKey);
  settings->endGroup();

  if (isServer.isNull()) {
    return false;
  }

  return isServer.toBool();
}
}  // namespace srilakshmikanthanp::clipbirdesk::storage
