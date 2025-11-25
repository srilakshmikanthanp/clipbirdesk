#include "server_manager.hpp"

#include "syncing/server_factory.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {

void ServerManager::onSyncingPacket(Session* session, const packets::SyncingPacket& packet) {
  if (!session->isTrusted()) return;
  QVector<QPair<QString, QByteArray>> items;
  for (const auto& item : packet.getItems()) {
    items.append(qMakePair(QString::fromUtf8(item.getMimeType()), item.getPayload()));
  }
  this->OnSyncRequest(items);
}

void ServerManager::onPingPongPacket(Session* session, const packets::PingPongPacket& packet) {
  if (packet.getPingType() == common::types::enums::Ping) {
    session->sendPacket(utility::functions::createPacket(utility::functions::params::PingPacketParams{.pingType = common::types::enums::Pong}));
  }
}

void ServerManager::onClientDisconnected(Session* session) {
  clients.removeOne(session);
  emit clientDisconnected(session);
}

void ServerManager::onClientConnected(Session* session) {
  clients.append(session);
  emit clientConnected(session);
}

void ServerManager::onClientError(Session* session, std::exception_ptr eptr) {
  emit errorOccurred(session, eptr);
}

void ServerManager::onServiceRegistered() {
  emit serviceRegistered();
}

void ServerManager::onServiceUnregistered() {
  emit serviceUnregistered();
}

void ServerManager::onServiceRegistrationFailed(std::exception_ptr eptr) {
  emit serviceRegisteringFailed(eptr);
}

void ServerManager::onServiceUnregistrationFailed(std::exception_ptr eptr) {
  emit serviceUnregisteringFailed(eptr);
}

void ServerManager::onNetworkPacket(Session* session, const packets::NetworkPacket& networkPacket) {
  if (auto syncPacket = dynamic_cast<const packets::SyncingPacket*>(&networkPacket)) {
    onSyncingPacket(session, *syncPacket);
  } else if (auto pingPacket = dynamic_cast<const packets::PingPongPacket*>(&networkPacket)) {
    onPingPongPacket(session, *pingPacket);
  }
}

void ServerManager::synchronize(const QVector<QPair<QString, QByteArray>>& items) {
  auto syncingPacket = utility::functions::createPacket(utility::functions::params::SyncingPacketParams{.items = items});
  for (auto* client : clients) {
    if (client->isTrusted()) {
      client->sendPacket(syncingPacket);
    }
  }
}

void ServerManager::start(bool useBluetooth) {
  if (server != nullptr) {
    throw std::runtime_error("ServerManager is already started");
  }

  if (useBluetooth) {
    server = ServerFactory::createBluetoothServer(this);
  } else {
    server = ServerFactory::createNetworkServer(this);
  }

  connect(server, &Server::onClientConnected, this, &ServerManager::onClientConnected);
  connect(server, &Server::onClientDisconnected, this, &ServerManager::onClientDisconnected);
  connect(server, &Server::onClientError, this, &ServerManager::onClientError);
  connect(server, &Server::onNetworkPacket, this, &ServerManager::onNetworkPacket);
  connect(server, &Server::onServiceRegistered, this, &ServerManager::onServiceRegistered);
  connect(server, &Server::onServiceUnregistered, this, &ServerManager::onServiceUnregistered);
  connect(server, &Server::onServiceRegistrationFailed, this, &ServerManager::onServiceRegistrationFailed);
  connect(server, &Server::onServiceUnregistrationFailed, this, &ServerManager::onServiceUnregistrationFailed);

  server->start();
}

void ServerManager::stop() {
  if (server == nullptr) {
    throw std::runtime_error("ServerManager is not started");
  }

  server->stop();
  server->deleteLater();
  server = nullptr;
}

ServerManager::ServerManager(QObject* parent) : HostManager(parent) {}

ServerManager::~ServerManager() {
  if (server != nullptr) {
    stop();
  }
}

}  // namespace srilakshmikanthanp::clipbirdesk::syncing
