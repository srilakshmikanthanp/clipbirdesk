#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <optional>

#include "syncing/manager/client_manager.hpp"
#include "syncing/manager/server_manager.hpp"
#include "syncing/manager/host_manager.hpp"
#include "syncing/client_server.hpp"
#include "syncing/session.hpp"
#include "syncing/synchronizer.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class SyncingManager : public Synchronizer {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(SyncingManager)

 private:
  // Managers
  ClientManager* clientManager = new ClientManager(this);
  ServerManager* serverManager = new ServerManager(this);
  HostManager* hostManager = nullptr;
  Session* connectedServer = nullptr;

  // State
  QVector<ClientServer*> availableServers;
  QVector<Session*> connectedClients;

 private:
  // ClientManager event handlers
  void onServerFound(ClientServer* server);
  void onServerGone(ClientServer* server);
  void onBrowsingStarted();
  void onBrowsingStopped();
  void onBrowsingStartFailed(std::exception_ptr eptr);
  void onBrowsingStopFailed(std::exception_ptr eptr);
  void onClientConnected(Session* session);
  void onClientDisconnected(Session* session);
  void onServerConnected(Session* session);
  void onServerDisconnected(Session* session);
  void onClientError(Session* session, std::exception_ptr eptr);
  void onServerError(Session* session, std::exception_ptr eptr);

  // ServerManager event handlers
  void onServiceRegistered();
  void onServiceUnregistered();
  void onServiceRegisteringFailed(std::exception_ptr eptr);
  void onServiceUnregisteringFailed(std::exception_ptr eptr);

 public:
  explicit SyncingManager(QObject* parent = nullptr);
  virtual ~SyncingManager();

  // Synchronizer interface
  virtual void synchronize(const QVector<QPair<QString, QByteArray>>& items) override;

  // Host management
  void setHostAsServer(bool useBluetooth = false);
  void setHostAsClient(bool useBluetooth = false);

  // stop
  void stop();

  // Connection management
  void connectToServer(ClientServer* server);

  // Getters
  std::optional<ClientServer*> getClientServerByName(const QString& name) const;
  std::optional<Session*> getServerClientSessionByName(const QString& name) const;
  QVector<ClientServer*> getAvailableServers() const;
  QVector<Session*> getConnectedClients() const;
  Session* getConnectedServer() const;
  HostManager* getHostManager() const;
  bool isHostServer() const;
  bool isBrowsing() const;

 signals:
  // ClientManager events
  void browsingStartFailedEvent(std::exception_ptr eptr);
  void browsingStopFailedEvent(std::exception_ptr eptr);
  void serverFoundEvent(ClientServer* server);
  void serverGoneEvent(ClientServer* server);
  void browsingStartedEvent();
  void browsingStoppedEvent();
  void connectedToServer(Session* session);
  void disconnectedFromServer(Session* session);
  void errorEvent(Session* session, std::exception_ptr eptr);

  // ServerManager events
  void clientDisconnectedEvent(Session* session);
  void clientConnectedEvent(Session* session);
  void serviceRegisteredEvent();
  void serviceUnregisteredEvent();
  void serviceRegisteringFailedEvent(std::exception_ptr eptr);
  void serviceUnregisteringFailedEvent(std::exception_ptr eptr);

  // State change events
  void availableServersChanged(QVector<ClientServer*> servers);
  void connectedClientsChanged(QVector<Session*> clients);
  void connectedServerChanged(Session* server);
  void hostManagerChanged(HostManager* manager);
  void isHostServerChanged(bool isServer);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
