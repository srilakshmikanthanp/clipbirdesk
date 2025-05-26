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
 * @brief Get All Client Certificates
 */
QList<QByteArray> Storage::getAllClientCert() {
  QList<QByteArray> list;

  settings->beginGroup(clientGroup);
  for (auto &name : settings->childKeys()) {
    auto cert = settings->value(name);
    list.append(cert.toByteArray());
  }
  settings->endGroup();

  return list;
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
 * @brief Clear the client cert
 */
void Storage::clearClientCert(const QString &name) {
  settings->beginGroup(clientGroup);
  settings->remove(name);
  settings->endGroup();
}

/**
 * @brief Clear the client cert
 */
void Storage::clearAllClientCert() {
  settings->beginGroup(clientGroup);
  settings->remove("");
  settings->endGroup();
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
 * @brief Get All Server Certificates
 */
QList<QByteArray> Storage::getAllServerCert() {
  QList<QByteArray> list;

  settings->beginGroup(serverGroup);
  for (auto &name : settings->childKeys()) {
    auto cert = settings->value(name);
    list.append(cert.toByteArray());
  }
  settings->endGroup();

  return list;
}

/**
 * @brief Clear the server cert
 */
void Storage::clearServerCert(const QString &name) {
  settings->beginGroup(serverGroup);
  settings->remove(name);
  settings->endGroup();
}

/**
 * @brief Clear the server cert
 */
void Storage::clearAllServerCert() {
  settings->beginGroup(serverGroup);
  settings->remove("");
  settings->endGroup();
}

/**
 * @brief set the Host certificate
 */
void Storage::setHostCert(const QSslCertificate &cert) {
  settings->beginGroup(commonGroup);
  settings->setValue(hostCertificateKey, cert.toPem());
  settings->endGroup();
}

/**
 * @brief has Host cert
 */
bool Storage::hasHostCert() {
  settings->beginGroup(commonGroup);
  auto cert = QSslCertificate(settings->value(hostCertificateKey).toByteArray());
  settings->endGroup();
  return !cert.isNull();
}

/**
 * @brief Get the Host certificate
 */
QSslCertificate Storage::getHostCert() {
  settings->beginGroup(commonGroup);
  auto cert = QSslCertificate(settings->value(hostCertificateKey).toByteArray());
  settings->endGroup();

  if (cert.isNull()) {
    throw std::invalid_argument("name not found");
  }

  return cert;
}

/**
 * @brief set the Host Key
 */
void Storage::setHostKey(const QSslKey &key) {
  settings->beginGroup(commonGroup);
  settings->setValue(hostKeyKey, key.toPem());
  settings->endGroup();
}

/**
 * @brief has Host Key
 */
bool Storage::hasHostKey() {
  settings->beginGroup(commonGroup);
  auto key = QSslKey(settings->value(hostKeyKey).toByteArray(), QSsl::Rsa);
  settings->endGroup();
  return !key.isNull();
}

/**
 * @brief Get the Host Key
 */
QSslKey Storage::getHostKey() {
  settings->beginGroup(commonGroup);
  auto key = QSslKey(settings->value(hostKeyKey).toByteArray(), QSsl::Rsa);
  settings->endGroup();

  if (key.isNull()) {
    throw std::invalid_argument("name not found");
  }

  return key;
}

/**
 * @brief Set the current state of the server or client
 *
 * @param isServer
 */
void Storage::setHostIsServer(bool isServer) {
  settings->beginGroup(commonGroup);
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
  settings->beginGroup(commonGroup);
  auto isServer = settings->value(hostStateKey);
  settings->endGroup();

  if (isServer.isNull()) {
    return false;
  }
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
