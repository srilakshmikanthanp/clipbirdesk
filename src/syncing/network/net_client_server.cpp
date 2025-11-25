#include "net_client_server.hpp"

#include "net_client_server_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
NetClientServer::NetClientServer(
  const NetResolvedDevice& device,
  const common::types::SslConfig& sslConfig,
  common::trust::TrustedServers* trustedServers,
  QObject* parent
) : ClientServer(device.name),
  netResolvedDevice(device),
  sslConfig(sslConfig),
  trustedServers(trustedServers) {
  // No operation
}

NetClientServer::~NetClientServer() {
  // No operation
}

void NetClientServer::connect() {
  auto session = new NetClientServerSession(
    trustedServers,
    netResolvedDevice,
    sslConfig,
    this
  );

  QObject::connect(
    session,
    &NetClientServerSession::networkPacket,
    this,
    &NetClientServer::onNetworkPacket
  );

  QObject::connect(
    session,
    &NetClientServerSession::connected,
    this,
    &NetClientServer::onConnected
  );

  QObject::connect(
    session,
    &NetClientServerSession::disconnected,
    this,
    &NetClientServer::onDisconnected
  );

  QObject::connect(
    session,
    &NetClientServerSession::error,
    this,
    &NetClientServer::onError
  );

  session->connect();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
