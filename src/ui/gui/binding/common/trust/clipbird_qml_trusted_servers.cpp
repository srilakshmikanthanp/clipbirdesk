#include "clipbird_qml_trusted_servers.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
void ClipbirdQmlTrustedServers::handleTrustedServersChanged(const QMap<QString, QByteArray>& servers) {
  QVariantList variantServers;
  for (auto it = servers.begin(); it != servers.end(); ++it) {
    QVariantMap server;
    server[devieName] = it.key();
    server[certificate] = it.value().toBase64();
    variantServers.append(server);
  }
  emit trustedServersChanged(variantServers);
}

/**
 * @brief Construct a new ClipbirdQmlTrustedServers object
 * @param trustedServers Pointer to TrustedServers implementation
 * @param parent Parent QObject
 */
ClipbirdQmlTrustedServers::ClipbirdQmlTrustedServers(common::trust::TrustedServers* trustedServers, QObject* parent)
    : QObject(parent), m_trustedServers(trustedServers) {
  connect(m_trustedServers, &common::trust::TrustedServers::trustedServersChanged, this, &ClipbirdQmlTrustedServers::handleTrustedServersChanged);
}

/**
 * @brief Destroy the ClipbirdQmlTrustedServers object
 */
ClipbirdQmlTrustedServers::~ClipbirdQmlTrustedServers() = default;

/**
 * @brief Get all trusted servers
 * @return QVariantList of objects with deviceName and certificate (as base64 strings)
 */
QVariantList ClipbirdQmlTrustedServers::getTrustedServers() const {
  QMap<QString, QByteArray> servers = m_trustedServers->getTrustedServers();
  QVariantList variantServers;

  for (auto it = servers.begin(); it != servers.end(); ++it) {
    QVariantMap server;
    server[devieName] = it.key();
    server[certificate] = it.value().toBase64();
    variantServers.append(server);
  }

  return variantServers;
}

/**
 * @brief Check if a server with given name and certificate is trusted
 * @param name Server name
 * @param certificate Certificate as base64 string
 * @return true if server is trusted, false otherwise
 */
bool ClipbirdQmlTrustedServers::isTrustedServer(const QString& name, const QString& certificate) const {
  QByteArray certBytes = QByteArray::fromBase64(certificate.toUtf8());
  return m_trustedServers->isTrustedServer(name, certBytes);
}

/**
 * @brief Check if a server with given name exists
 * @param name Server name
 * @return true if server exists, false otherwise
 */
bool ClipbirdQmlTrustedServers::hasTrustedServer(const QString& name) const {
  return m_trustedServers->hasTrustedServer(name);
}

/**
 * @brief Add a trusted server
 * @param name Server name
 * @param certificate Certificate as base64 string
 */
void ClipbirdQmlTrustedServers::addTrustedServer(const QString& name, const QString& certificate) {
  QByteArray certBytes = QByteArray::fromBase64(certificate.toUtf8());
  m_trustedServers->addTrustedServer(name, certBytes);
}

/**
 * @brief Remove a trusted server
 * @param name Server name
 */
void ClipbirdQmlTrustedServers::removeTrustedServer(const QString& name) {
  m_trustedServers->removeTrustedServer(name);
}

/**
 * @brief Get singleton instance for QML
 * @param engine QML engine (not used)
 * @param scriptEngine JS engine (not used)
 * @return ClipbirdQmlTrustedServers* Singleton instance
 */
ClipbirdQmlTrustedServers* ClipbirdQmlTrustedServers::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  static ClipbirdQmlTrustedServers* instance = new ClipbirdQmlTrustedServers(common::trust::TrustedServersFactory::getTrustedServers());
  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
