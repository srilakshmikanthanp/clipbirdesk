#include "clipbird_qml_trusted_clients.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
void ClipbirdQmlTrustedClients::handleTrustedClientsChanged(const QMap<QString, QByteArray>& clients) {
  QVariantList variantClients;
  for (auto it = clients.begin(); it != clients.end(); ++it) {
    QVariantMap client;
    client[devieName] = it.key();
    client[certificate] = it.value().toBase64();
    variantClients.append(client);
  }
  emit trustedClientsChanged(variantClients);
}

/**
 * @brief Construct a new ClipbirdQmlTrustedClients object
 * @param trustedClients Pointer to TrustedClients implementation
 * @param parent Parent QObject
 */
ClipbirdQmlTrustedClients::ClipbirdQmlTrustedClients(common::trust::TrustedClients* trustedClients, QObject* parent)
    : QObject(parent), m_trustedClients(trustedClients) {
  connect(m_trustedClients, &common::trust::TrustedClients::trustedClientsChanged, this, &ClipbirdQmlTrustedClients::handleTrustedClientsChanged);
}

/**
 * @brief Destroy the ClipbirdQmlTrustedClients object
 */
ClipbirdQmlTrustedClients::~ClipbirdQmlTrustedClients() = default;

/**
 * @brief Get all trusted clients
 * @return QVariantList of objects with deviceName and certificate (as base64 strings)
 */
QVariantList ClipbirdQmlTrustedClients::getTrustedClients() const {
  QMap<QString, QByteArray> clients = m_trustedClients->getTrustedClients();
  QVariantList variantClients;

  for (auto it = clients.begin(); it != clients.end(); ++it) {
    QVariantMap client;
    client[devieName] = it.key();
    client[certificate] = it.value().toBase64();
    variantClients.append(client);
  }

  return variantClients;
}

/**
 * @brief Check if a client with given name and certificate is trusted
 * @param name Client name
 * @param certificate Certificate as base64 string
 * @return true if client is trusted, false otherwise
 */
bool ClipbirdQmlTrustedClients::isTrustedClient(const QString& name, const QString& certificate) const {
  QByteArray certBytes = QByteArray::fromBase64(certificate.toUtf8());
  return m_trustedClients->isTrustedClient(name, certBytes);
}

/**
 * @brief Check if a client with given name exists
 * @param name Client name
 * @return true if client exists, false otherwise
 */
bool ClipbirdQmlTrustedClients::hasTrustedClient(const QString& name) const {
  return m_trustedClients->hasTrustedClient(name);
}

/**
 * @brief Add a trusted client
 * @param name Client name
 * @param certificate Certificate as base64 string
 */
void ClipbirdQmlTrustedClients::addTrustedClient(const QString& name, const QString& certificate) {
  QByteArray certBytes = QByteArray::fromBase64(certificate.toUtf8());
  m_trustedClients->addTrustedClient(name, certBytes);
}

/**
 * @brief Remove a trusted client
 * @param name Client name
 */
void ClipbirdQmlTrustedClients::removeTrustedClient(const QString& name) {
  m_trustedClients->removeTrustedClient(name);
}

/**
 * @brief Get singleton instance for QML
 * @param engine QML engine (not used)
 * @param scriptEngine JS engine (not used)
 * @return ClipbirdQmlTrustedClients* Singleton instance
 */
ClipbirdQmlTrustedClients* ClipbirdQmlTrustedClients::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);

  static ClipbirdQmlTrustedClients* instance = new ClipbirdQmlTrustedClients(
    common::trust::TrustedClientsFactory::getTrustedClients()
  );

  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
