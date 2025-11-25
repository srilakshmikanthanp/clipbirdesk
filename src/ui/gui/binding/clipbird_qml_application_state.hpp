#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

// project headers
#include "application_state.hpp"
#include "application_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for ApplicatiionState that exposes application state to QML
 * This wraps the ApplicatiionState interface and makes it accessible from QML as a singleton
 */
class ClipbirdQmlApplicationState : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(bool isServer READ getIsServer WRITE setIsServer NOTIFY isServerChanged)
  Q_PROPERTY(bool useBluetooth READ shouldUseBluetooth WRITE setUseBluetooth NOTIFY useBluetoothChanged)
  Q_PROPERTY(bool hasHostSslConfig READ hasHostSslConfig NOTIFY hostSslConfigChanged)

private:
  ApplicatiionState* m_applicationState = nullptr;

  static constexpr const char* privateKeyKey = "privateKey";
  static constexpr const char* certificateKey = "certificate";

signals:
  /**
   * @brief Signal emitted when host SSL config changes
   * @param config SSL config as variant map (privateKey and certificate as base64 strings)
   */
  void hostSslConfigChanged(QVariantMap config);

  /**
   * @brief Signal emitted when isServer changes
   * @param isServer Whether the application is running as server
   */
  void isServerChanged(bool isServer);

  /**
   * @brief Signal emitted when useBluetooth changes
   * @param useBluetooth Whether bluetooth should be used
   */
  void useBluetoothChanged(bool useBluetooth);

public:
  /**
   * @brief Get singleton instance for QML
   * @param engine QML engine (not used)
   * @param scriptEngine JS engine (not used)
   * @return ClipbirdQmlApplicationState* Singleton instance
   */
  static ClipbirdQmlApplicationState* create(QQmlEngine* engine, QJSEngine* scriptEngine);

private:
  /**
   * @brief Construct a new ClipbirdQmlApplicationState object
   * @param applicationState Pointer to ApplicatiionState implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlApplicationState(ApplicatiionState* applicationState, QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlApplicationState object
   */
  virtual ~ClipbirdQmlApplicationState();

private slots:
  /**
   * @brief Handle host SSL config changes from application state
   * @param sslConfig The new SSL configuration
   */
  void handleHostSslConfigChanged(const std::optional<common::types::SslConfig>& sslConfig);

public:
  /**
   * @brief Get host SSL config
   * @return QVariantMap containing privateKey and certificate as base64 strings (empty if not set)
   */
  Q_INVOKABLE QVariantMap getHostSslConfig() const;

  /**
   * @brief Set host SSL config
   * @param config Map containing privateKey and certificate as base64 strings
   */
  Q_INVOKABLE void setHostSslConfig(const QVariantMap& config);

  /**
   * @brief Clear host SSL config
   */
  Q_INVOKABLE void clearHostSslConfig();

  /**
   * @brief Check if host SSL config is set
   * @return true if SSL config is set, false otherwise
   */
  Q_INVOKABLE bool hasHostSslConfig() const;

  /**
   * @brief Get whether application is running as server
   * @return true if server, false if client
   */
  Q_INVOKABLE bool getIsServer() const;

  /**
   * @brief Set whether application is running as server
   * @param isServer true for server, false for client
   */
  Q_INVOKABLE void setIsServer(bool isServer);

  /**
   * @brief Get whether bluetooth should be used
   * @return true if bluetooth enabled, false otherwise
   */
  Q_INVOKABLE bool shouldUseBluetooth() const;

  /**
   * @brief Set whether bluetooth should be used
   * @param useBluetooth true to enable bluetooth, false to disable
   */
  Q_INVOKABLE void setUseBluetooth(bool useBluetooth);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
