#include "client_manager.hpp"

#include "syncing/client_server_browser_factory.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
void ClientManager::handleAuthenticationPacket(Session* session, const packets::Authentication& packet) {
  if (packet.getAuthStatus() == common::types::enums::AuthStatus::AuthOkay) {
    emit connected(session);
  } else {
    session->disconnectFromHost();
  }
}

void ClientManager::handleInvalidRequestPacket(Session* session, const packets::InvalidRequest& packet) {
  emit handleError(session, std::make_exception_ptr(packets::InvalidRequestException(packet)));
}

void ClientManager::handleSyncingPacket(Session* session, const packets::SyncingPacket& packet) {
  if (!session->isTrusted()) return;
  QVector<QPair<QString, QByteArray>> items;
  for (const auto& item : packet.getItems()) {
    items.append(qMakePair(QString::fromUtf8(item.getMimeType()), item.getPayload()));
  }
  this->OnSyncRequest(items);
}

void ClientManager::handlePingPongPacket(Session* session, const packets::PingPongPacket& packet) {
  session->sendPacket(utility::functions::createPacket(utility::functions::params::PingPacketParams{.pingType = common::types::enums::Pong}));
}

void ClientManager::handleServerFound(ClientServer *server) {
  emit serverFound(server);
}

void ClientManager::handleServerGone(ClientServer *server) {
  emit serverGone(server);
}

void ClientManager::handleBrowsingStarted() {
  emit browsingStarted();
}

void ClientManager::handleBrowsingStopped() {
  emit browsingStopped();
}

void ClientManager::handleBrowsingStartFailed(std::exception_ptr eptr) {
  emit browsingStartFailed(eptr);
}

void ClientManager::handleBrowsingStopFailed(std::exception_ptr eptr) {
  emit browsingStopFailed(eptr);
}

void ClientManager::handleConnected(Session *session) {
  this->session = session;
  this->session->setParent(this);
}

void ClientManager::handleDisconnected(Session *session) {
  this->session = nullptr;
  emit disconnected(session);
}

void ClientManager::handleError(Session *session, std::exception_ptr eptr) {
  emit errorOccurred(session, eptr);
}

void ClientManager::handleNetworkPacket(Session* session, const packets::NetworkPacket& networkPacket) {
  if (auto authPacket = dynamic_cast<const packets::Authentication*>(&networkPacket)) {
    handleAuthenticationPacket(session, *authPacket);
  } else if (auto invalidPacket = dynamic_cast<const packets::InvalidRequest*>(&networkPacket)) {
    handleInvalidRequestPacket(session, *invalidPacket);
  } else if (auto syncPacket = dynamic_cast<const packets::SyncingPacket*>(&networkPacket)) {
    handleSyncingPacket(session, *syncPacket);
  } else if (auto pingPacket = dynamic_cast<const packets::PingPongPacket*>(&networkPacket)) {
    handlePingPongPacket(session, *pingPacket);
  }
}

void ClientManager::synchronize(const QVector<QPair<QString, QByteArray>>& items) {
  if (session != nullptr && session->isTrusted()) {
    session->sendPacket(utility::functions::createPacket(utility::functions::params::SyncingPacketParams{.items = items}));
  }
}

Session* ClientManager::getSession() {
  return this->session;
}

void ClientManager::connectToServer(ClientServer* server) {
  QObject::connect(
    server,
    &ClientServer::onNetworkPacket,
    this,
    &ClientManager::handleNetworkPacket
  );

  QObject::connect(
    server,
    &ClientServer::onConnected,
    this,
    &ClientManager::handleConnected
  );

  QObject::connect(
    server,
    &ClientServer::onDisconnected,
    this,
    &ClientManager::handleDisconnected
  );

  QObject::connect(
    server,
    &ClientServer::onError,
    this,
    &ClientManager::handleError
  );

  server->connect();
}

void ClientManager::start(bool useBluetooth) {
  if (this->clientServerBrowser != nullptr) {
    throw std::runtime_error("ClientManager is already started");
  }
  if (useBluetooth) {
    this->clientServerBrowser = ClientServerBrowserFactory::createBluetoothClientServerBrowser(this);
  } else {
    this->clientServerBrowser = ClientServerBrowserFactory::createNetworkClientServerBrowser(this);
  }
  connect(this->clientServerBrowser, &ClientServerBrowser::onBrowsingStartFailed, this, &ClientManager::handleBrowsingStartFailed);
  connect(this->clientServerBrowser, &ClientServerBrowser::onBrowsingStopFailed, this, &ClientManager::handleBrowsingStopFailed);
  connect(this->clientServerBrowser, &ClientServerBrowser::onServerFound, this, &ClientManager::handleServerFound);
  connect(this->clientServerBrowser, &ClientServerBrowser::onServerGone, this, &ClientManager::handleServerGone);
  connect(this->clientServerBrowser, &ClientServerBrowser::onBrowsingStarted, this, &ClientManager::handleBrowsingStarted);
  connect(this->clientServerBrowser, &ClientServerBrowser::onBrowsingStopped, this, &ClientManager::handleBrowsingStopped);
  this->clientServerBrowser->start();
}

void ClientManager::stop() {
  if (this->clientServerBrowser == nullptr) {
    throw std::runtime_error("ClientManager is not started");
  }
  this->session->disconnectFromHost();
  this->clientServerBrowser->stop();
  this->clientServerBrowser->deleteLater();
  this->clientServerBrowser = nullptr;
}

ClientManager::ClientManager(QObject* parent): HostManager(parent) {}

ClientManager::~ClientManager() {
  if (this->clientServerBrowser != nullptr) {
    this->stop();
  }
}
}
