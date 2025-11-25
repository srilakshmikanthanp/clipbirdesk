#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QtQml/qqmlregistration.h>

#include <QByteArray>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

// project headers
#include "common/trust/trusted_clients.hpp"
#include "common/trust/trusted_clients_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for TrustedClients that exposes trusted clients management to QML
 * This wraps the TrustedClients interface and makes it accessible from QML as a singleton
 */
class ClipbirdQmlTrustedClients : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

 private:
  static inline const char* devieName = "deviceName";
  static inline const char* certificate = "certificate";

 private:

  common::trust::TrustedClients* m_trustedClients = nullptr;

 private:

  void handleTrustedClientsChanged(const QMap<QString, QByteArray>& clients);

 signals:
  /**
   * @brief Signal emitted when trusted clients change
   * @param clients List of client objects with deviceName and certificate
   */
  void trustedClientsChanged(QVariantList clients);

 public:

  /**
   * @brief Construct a new ClipbirdQmlTrustedClients object
   * @param trustedClients Pointer to TrustedClients implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlTrustedClients(
      common::trust::TrustedClients* trustedClients, QObject* parent = nullptr
  );

  /**
   * @brief Destroy the ClipbirdQmlTrustedClients object
   */
  virtual ~ClipbirdQmlTrustedClients();

  /**
   * @brief Get all trusted clients
   * @return QVariantList of objects with deviceName and certificate (as base64 strings)
   */
  Q_INVOKABLE QVariantList getTrustedClients() const;

  /**
   * @brief Check if a client with given name and certificate is trusted
   * @param name Client name
   * @param certificate Certificate as base64 string
   * @return true if client is trusted, false otherwise
   */
  Q_INVOKABLE bool isTrustedClient(const QString& name, const QString& certificate) const;

  /**
   * @brief Check if a client with given name exists
   * @param name Client name
   * @return true if client exists, false otherwise
   */
  Q_INVOKABLE bool hasTrustedClient(const QString& name) const;

  /**
   * @brief Add a trusted client
   * @param name Client name
   * @param certificate Certificate as base64 string
   */
  Q_INVOKABLE void addTrustedClient(const QString& name, const QString& certificate);

  /**
   * @brief Remove a trusted client
   * @param name Client name
   */
  Q_INVOKABLE void removeTrustedClient(const QString& name);

 public:

  /**
   * @brief Get singleton instance for QML
   * @param engine QML engine (not used)
   * @param scriptEngine JS engine (not used)
   * @return ClipbirdQmlTrustedClients* Singleton instance
   */
  static ClipbirdQmlTrustedClients* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
