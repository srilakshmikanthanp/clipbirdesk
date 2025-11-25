#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QObject>
#include <QString>
#include <QList>
#include <QtQml/qqmlregistration.h>
#include <QQmlEngine>
#include <QJSEngine>

// project headers
#include "syncing/manager/syncing_manager.hpp"
#include "syncing/client_server.hpp"
#include "syncing/session.hpp"
#include "ui/gui/binding/syncing/clipbird_qml_session.hpp"
#include "ui/gui/binding/syncing/clipbird_qml_client_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

/**
 * @brief QML binding for SyncingManager that exposes syncing functionality to QML
 * This wraps the SyncingManager and makes it accessible from QML as a singleton
 */
class ClipbirdQmlSyncingManager : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

  Q_PROPERTY(QList<ClipbirdQmlSession*> connectedClients READ getConnectedClients NOTIFY connectedClientsChanged)
  Q_PROPERTY(ClipbirdQmlSession* connectedServer READ getConnectedServer NOTIFY connectedServerChanged)
  Q_PROPERTY(QList<ClipbirdQmlClientServer*> availableServers READ getAvailableServers NOTIFY availableServersChanged)

 private:
  syncing::SyncingManager* m_syncingManager = nullptr;
  QList<ClipbirdQmlSession*> m_connectedClients;
  ClipbirdQmlSession* m_connectedServer = nullptr;
  QList<ClipbirdQmlClientServer*> m_availableServers;

 private:
  void putConnectedClients(QVector<syncing::Session*> clients);
  void putConnectedServer(syncing::Session* server);
  void putAvailableServers(QVector<syncing::ClientServer*> servers);

 signals:
  void connectedClientsChanged();
  void connectedServerChanged();
  void availableServersChanged();

 public:
  /**
   * @brief Construct a new ClipbirdQmlSyncingManager object
   * @param syncingManager Pointer to SyncingManager implementation
   * @param parent Parent QObject
   */
  explicit ClipbirdQmlSyncingManager(syncing::SyncingManager* syncingManager, QObject* parent = nullptr);

  /**
   * @brief Destroy the ClipbirdQmlSyncingManager object
   */
  virtual ~ClipbirdQmlSyncingManager();

  /**
   * @brief Get connected clients
   * @return QList<ClipbirdQmlSession*> List of client sessions
   */
  QList<ClipbirdQmlSession*> getConnectedClients() const;

  /**
   * @brief Get connected server
   * @return ClipbirdQmlSession* Connected server session or nullptr
   */
  ClipbirdQmlSession* getConnectedServer() const;

  /**
   * @brief Get available servers
   * @return QList<ClipbirdQmlClientServer*> List of available servers
   */
  QList<ClipbirdQmlClientServer*> getAvailableServers() const;

  /**
   * @brief Connect to a server by name
   * @param serverName Server name to connect to
   */
  Q_INVOKABLE void connectToServer(const QString& serverName);

  /**
   * @brief Disconnect from current server
   */
  Q_INVOKABLE void disconnectFromServer();

  /**
   * @brief Disconnect a client by name
   * @param clientName Client name to disconnect
   */
  Q_INVOKABLE void disconnectClient(const QString& clientName);

  /**
   * @brief Set host as client
   * @param useBluetooth Whether to use Bluetooth
   */
  Q_INVOKABLE void setHostAsClient(bool useBluetooth = false);


  /**
   * @brief Set host as server
   * @param useBluetooth Whether to use Bluetooth
   */
  Q_INVOKABLE void setHostAsServer(bool useBluetooth = false);

  /**
   * synchronize
   */
  Q_INVOKABLE void synchronize(const QVariantList& items);

 public:
  /**
   * @brief Get singleton instance for QML
   * @param engine QML engine (not used)
   * @param scriptEngine JS engine (not used)
   * @return ClipbirdQmlSyncingManager* Singleton instance
   */
  static ClipbirdQmlSyncingManager* create(QQmlEngine* engine, QJSEngine* scriptEngine);
};

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
