#include "clipbird_qml_application_state.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {
/**
 * @brief Construct a new ClipbirdQmlApplicationState object
 * @param applicationState Pointer to ApplicatiionState implementation
 * @param parent Parent QObject
 */
ClipbirdQmlApplicationState::ClipbirdQmlApplicationState(ApplicatiionState* applicationState, QObject* parent): QObject(parent), m_applicationState(applicationState) {
  connect(
    m_applicationState,
    &ApplicatiionState::onHostSSlConfigChanged,
    this,
    &ClipbirdQmlApplicationState::handleHostSslConfigChanged
  );

  connect(
    m_applicationState,
    &ApplicatiionState::onIsServerChanged,
    this,
    &ClipbirdQmlApplicationState::isServerChanged
  );

  connect(
    m_applicationState,
    &ApplicatiionState::shouldUseBluetoothChanged,
    this,
    &ClipbirdQmlApplicationState::useBluetoothChanged
  );
}

/**
 * @brief Destroy the ClipbirdQmlApplicationState object
 */
ClipbirdQmlApplicationState::~ClipbirdQmlApplicationState() = default;

/**
 * @brief Handle host SSL config changes from application state
 * @param sslConfig The new SSL configuration
 */
void ClipbirdQmlApplicationState::handleHostSslConfigChanged(const std::optional<common::types::SslConfig>& sslConfig) {
  QVariantMap config;
  if (sslConfig.has_value()) {
    config[privateKeyKey] = sslConfig->privateKey.toBase64();
    config[certificateKey] = sslConfig->certificate.toBase64();
  }
  emit hostSslConfigChanged(config);
}

/**
 * @brief Get host SSL config
 * @return QVariantMap containing privateKey and certificate as base64 strings (empty if not set)
 */
QVariantMap ClipbirdQmlApplicationState::getHostSslConfig() const {
  auto sslConfig = m_applicationState->getHostSslConfig();
  QVariantMap config;

  if (sslConfig.has_value()) {
    config[privateKeyKey] = sslConfig->privateKey.toBase64();
    config[certificateKey] = sslConfig->certificate.toBase64();
  }

  return config;
}

/**
 * @brief Set host SSL config
 * @param config Map containing privateKey and certificate as base64 strings
 */
void ClipbirdQmlApplicationState::setHostSslConfig(const QVariantMap& config) {
  if (config.isEmpty() || !config.contains(privateKeyKey) || !config.contains(certificateKey)) {
    clearHostSslConfig();
    return;
  }

  common::types::SslConfig sslConfig;
  sslConfig.privateKey = QByteArray::fromBase64(config[privateKeyKey].toString().toUtf8());
  sslConfig.certificate = QByteArray::fromBase64(config[certificateKey].toString().toUtf8());

  m_applicationState->setHostSslConfig(sslConfig);
}

/**
 * @brief Clear host SSL config
 */
void ClipbirdQmlApplicationState::clearHostSslConfig() {
  m_applicationState->setHostSslConfig(std::nullopt);
}

/**
 * @brief Check if host SSL config is set
 * @return true if SSL config is set, false otherwise
 */
bool ClipbirdQmlApplicationState::hasHostSslConfig() const {
  auto sslConfig = m_applicationState->getHostSslConfig();
  return sslConfig.has_value() && !sslConfig->isNull();
}

/**
 * @brief Get whether application is running as server
 * @return true if server, false if client
 */
bool ClipbirdQmlApplicationState::getIsServer() const {
  return m_applicationState->getIsServer();
}

/**
 * @brief Set whether application is running as server
 * @param isServer true for server, false for client
 */
void ClipbirdQmlApplicationState::setIsServer(bool isServer) {
  m_applicationState->setIsServer(isServer);
}

/**
 * @brief Get whether bluetooth should be used
 * @return true if bluetooth enabled, false otherwise
 */
bool ClipbirdQmlApplicationState::shouldUseBluetooth() const {
  return m_applicationState->shouldUseBluetooth();
}

/**
 * @brief Set whether bluetooth should be used
 * @param useBluetooth true to enable bluetooth, false to disable
 */
void ClipbirdQmlApplicationState::setUseBluetooth(bool useBluetooth) {
  m_applicationState->setUseBluetooth(useBluetooth);
}

/**
 * @brief Get singleton instance for QML
 * @param engine QML engine (not used)
 * @param scriptEngine JS engine (not used)
 * @return ClipbirdQmlApplicationState* Singleton instance
 */
ClipbirdQmlApplicationState* ClipbirdQmlApplicationState::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);

  static ClipbirdQmlApplicationState* instance = new ClipbirdQmlApplicationState(
    ApplicationFactory::getApplicationState()
  );

  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
