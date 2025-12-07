#include "syncing_manager.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {

void SyncingManager::onServerFound(ClientServer* server) {
  availableServers.append(server);
  emit serverFoundEvent(server);
  emit availableServersChanged(availableServers);
}

void SyncingManager::onServerGone(ClientServer* server) {
  availableServers.removeOne(server);
  emit serverGoneEvent(server);
  emit availableServersChanged(availableServers);
}

void SyncingManager::onBrowsingStarted() {
  emit browsingStartedEvent();
}

void SyncingManager::onBrowsingStopped() {
  emit browsingStoppedEvent();
}

void SyncingManager::onBrowsingStartFailed(std::exception_ptr eptr) {
  emit browsingStartFailedEvent(eptr);
}

void SyncingManager::onBrowsingStopFailed(std::exception_ptr eptr) {
  emit browsingStopFailedEvent(eptr);
}

void SyncingManager::onServerConnected(Session* session) {
  emit connectedToServer(session);
  emit connectedServerChanged(session);
}

void SyncingManager::onServerDisconnected(Session* session) {
  emit disconnectedFromServer(session);
  emit connectedServerChanged(nullptr);
}

void SyncingManager::onServerError(Session* session, std::exception_ptr eptr) {
  emit errorEvent(session, eptr);
}

void SyncingManager::onClientDisconnected(Session* session) {
  connectedClients.removeOne(session);
  emit clientDisconnectedEvent(session);
  emit connectedClientsChanged(connectedClients);
}

void SyncingManager::onClientConnected(Session* session) {
  connectedClients.append(session);
  emit clientConnectedEvent(session);
  emit connectedClientsChanged(connectedClients);
}

void SyncingManager::onClientError(Session* session, std::exception_ptr eptr) {
  emit errorEvent(session, eptr);
}

void SyncingManager::onServiceRegistered() {
  emit serviceRegisteredEvent();
}

void SyncingManager::onServiceUnregistered() {
  emit serviceUnregisteredEvent();
}

void SyncingManager::onServiceRegisteringFailed(std::exception_ptr eptr) {
  emit serviceRegisteringFailedEvent(eptr);
}

void SyncingManager::onServiceUnregisteringFailed(std::exception_ptr eptr) {
  emit serviceUnregisteringFailedEvent(eptr);
}

void SyncingManager::synchronize(const QVector<QPair<QString, QByteArray>>& items) {
  if (hostManager != nullptr) {
    hostManager->synchronize(items);
  }
}

// Host management
void SyncingManager::setHostAsServer(bool useBluetooth) {
  for (auto* client : connectedClients) client->disconnectFromHost();
  connectedClients.clear();
  emit connectedClientsChanged(connectedClients);

  availableServers.clear();
  emit availableServersChanged(availableServers);

  if (hostManager != nullptr) {
    hostManager->stop();
  }

  hostManager = serverManager;
  hostManager->start(useBluetooth);
  emit hostManagerChanged(hostManager);
  emit isHostServerChanged(true);
}

void SyncingManager::setHostAsClient(bool useBluetooth) {
  for (auto* client : connectedClients) client->disconnectFromHost();
  connectedClients.clear();
  emit connectedClientsChanged(connectedClients);

  availableServers.clear();
  emit availableServersChanged(availableServers);

  if (hostManager != nullptr) {
    hostManager->stop();
  }

  hostManager = clientManager;
  hostManager->start(useBluetooth);
  emit hostManagerChanged(hostManager);
  emit isHostServerChanged(false);
}

void SyncingManager::connectToServer(ClientServer* server) {
  this->clientManager->connectToServer(server);
}

// Getters
std::optional<ClientServer*> SyncingManager::getClientServerByName(const QString& name) const {
  for (auto* server : availableServers) {
    if (server->getName() == name) return server;
  }
  return std::nullopt;
}

std::optional<Session*> SyncingManager::getServerClientSessionByName(const QString& name) const {
  for (auto* session : connectedClients) {
    if (session->getName() == name) return session;
  }
  return std::nullopt;
}

QVector<ClientServer*> SyncingManager::getAvailableServers() const {
  return availableServers;
}

QVector<Session*> SyncingManager::getConnectedClients() const {
  return connectedClients;
}

Session* SyncingManager::getConnectedServer() const {
  return this->clientManager->getSession();
}

HostManager* SyncingManager::getHostManager() const {
  return hostManager;
}

bool SyncingManager::isHostServer() const {
  return hostManager == serverManager;
}

SyncingManager::SyncingManager(QObject* parent) : Synchronizer(parent) {
  connect(serverManager, &ServerManager::clientConnected, this, &SyncingManager::onClientConnected);
  connect(serverManager, &ServerManager::clientDisconnected, this, &SyncingManager::onClientDisconnected);
  connect(serverManager, &ServerManager::serviceRegistered, this, &SyncingManager::onServiceRegistered);
  connect(serverManager, &ServerManager::serviceUnregistered, this, &SyncingManager::onServiceUnregistered);
  connect(serverManager, &ServerManager::serviceRegisteringFailed, this, &SyncingManager::onServiceRegisteringFailed);
  connect(serverManager, &ServerManager::serviceUnregisteringFailed, this, &SyncingManager::onServiceUnregisteringFailed);
  connect(serverManager, &ServerManager::errorOccurred, this, &SyncingManager::onClientError);
  connect(serverManager, &ServerManager::OnSyncRequest, this, &SyncingManager::OnSyncRequest);

  connect(clientManager, &ClientManager::serverFound, this, &SyncingManager::onServerFound);
  connect(clientManager, &ClientManager::serverGone, this, &SyncingManager::onServerGone);
  connect(clientManager, &ClientManager::browsingStarted, this, &SyncingManager::onBrowsingStarted);
  connect(clientManager, &ClientManager::browsingStopped, this, &SyncingManager::onBrowsingStopped);
  connect(clientManager, &ClientManager::browsingStartFailed, this, &SyncingManager::onBrowsingStartFailed);
  connect(clientManager, &ClientManager::browsingStopFailed, this, &SyncingManager::onBrowsingStopFailed);
  connect(clientManager, &ClientManager::connected, this, &SyncingManager::onServerConnected);
  connect(clientManager, &ClientManager::disconnected, this, &SyncingManager::onServerDisconnected);
  connect(clientManager, &ClientManager::errorOccurred, this, &SyncingManager::onServerError);
  connect(clientManager, &ClientManager::OnSyncRequest, this, &SyncingManager::OnSyncRequest);
}

SyncingManager::~SyncingManager() {
  if (hostManager != nullptr) {
    hostManager->stop();
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
