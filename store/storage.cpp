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
 * @brief Store Client name and JWT token
 *
 * @param name
 * @param token
 */
void Storage::setClientCert(const QString &name, const QByteArray &token) {
  settings->beginGroup(clientGroup);
  settings->setValue(name, token);
  settings->endGroup();
}

/**
 * @brief has the cert for the name
 */
bool Storage::hasClientCert(const QString &name) {
  settings->beginGroup(clientGroup);
  auto token = settings->value(name);
  settings->endGroup();

  return !token.isNull();
}

/**
 * @brief Get the JWT token for the name
 *
 * @param name
 * @return QString
 *
 * @throw std::invalid_argument if name not found
 */
QByteArray Storage::getClientCert(const QString &name) {
  settings->beginGroup(clientGroup);
  auto token = settings->value(name);
  settings->endGroup();

  if (token.isNull()) {
    throw std::invalid_argument("name not found");
  }

  return token.toByteArray();
}

/**
 * @brief Store the server name and JWT token
 *
 * @param name
 * @param token
 */
void Storage::setServerCert(const QString &name, const QByteArray &token) {
  settings->beginGroup(serverGroup);
  settings->setValue(name, token);
  settings->endGroup();
}

/**
 * @brief has the cert for the name
 */
bool Storage::hasServerCert(const QString &name) {
  settings->beginGroup(serverGroup);
  auto token = settings->value(name);
  settings->endGroup();

  return !token.isNull();
}

/**
 * @brief Get the JWT token for the name
 *
 * @param name
 * @return QString
 *
 * @throw std::invalid_argument if name not found
 */
QByteArray Storage::getServerCert(const QString &name) {
  settings->beginGroup(serverGroup);
  auto token = settings->value(name);
  settings->endGroup();

  if (token.isNull()) {
    throw std::invalid_argument("name not found");
  }

  return token.toByteArray();
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
  const auto s = isServer.toBool();
  return isServer.toBool();
}

/**
 * @brief Instance of the storage
 */
Storage& Storage::instance() {
  static Storage instance;
  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::storage
