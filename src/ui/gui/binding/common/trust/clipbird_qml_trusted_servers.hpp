#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVariantList>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

// project headers
#include "common/trust/trusted_servers.hpp"
#include "common/trust/trusted_servers_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for TrustedServers that exposes trusted servers management to QML
 * This wraps the TrustedServers interface and makes it accessible from QML as a singleton
 */
class ClipbirdQmlTrustedServers : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 private:
  static inline const char* devieName = "deviceName";
  static inline const char* certificate = "certificate";

private:
  common::trust::TrustedServers* m_trustedServers = nullptr;

private:
  void handleTrustedServersChanged(const QMap<QString, QByteArray>& servers);

signals:
  /**
   * @brief Signal emitted when trusted servers change
   * @param servers List of server objects with deviceName and certificate
   */
  void trustedServersChanged(QVariantList servers);

public:
  /**
   * @brief Construct a new ClipbirdQmlTrustedServers object
   * @param trustedServers Pointer to TrustedServers implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlTrustedServers(common::trust::TrustedServers* trustedServers, QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlTrustedServers object
   */
  virtual ~ClipbirdQmlTrustedServers();

  /**
   * @brief Get all trusted servers
   * @return QVariantList of objects with deviceName and certificate (as base64 strings)
   */
  Q_INVOKABLE QVariantList getTrustedServers() const;

  /**
   * @brief Check if a server with given name and certificate is trusted
   * @param name Server name
   * @param certificate Certificate as base64 string
   * @return true if server is trusted, false otherwise
   */
  Q_INVOKABLE bool isTrustedServer(const QString& name, const QString& certificate) const;

  /**
   * @brief Check if a server with given name exists
   * @param name Server name
   * @return true if server exists, false otherwise
   */
  Q_INVOKABLE bool hasTrustedServer(const QString& name) const;

  /**
   * @brief Add a trusted server
   * @param name Server name
   * @param certificate Certificate as base64 string
   */
  Q_INVOKABLE void addTrustedServer(const QString& name, const QString& certificate);

  /**
   * @brief Remove a trusted server
   * @param name Server name
   */
  Q_INVOKABLE void removeTrustedServer(const QString& name);

public:
  /**
   * @brief Get singleton instance for QML
   * @param engine QML engine (not used)
   * @param scriptEngine JS engine (not used)
   * @return ClipbirdQmlTrustedServers* Singleton instance
   */
  static ClipbirdQmlTrustedServers* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
