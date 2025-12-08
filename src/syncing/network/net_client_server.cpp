#include "net_client_server.hpp"

#include "net_client_server_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
NetClientServer::NetClientServer(
  const NetResolvedDevice& device,
  const common::types::SslConfig& sslConfig,
  common::trust::TrustedServers* trustedServers,
  QObject* parent
) : ClientServer(device.name, parent),
  netResolvedDevice(device),
  sslConfig(sslConfig),
  trustedServers(trustedServers) {
  // No operation
}

NetClientServer::~NetClientServer() {
  // No operation
}

void NetClientServer::connect(syncing::ClientServerEventHandler *handler) {
  auto session = new NetClientServerSession(
    trustedServers,
    netResolvedDevice,
    sslConfig,
    this
  );

  QObject::connect(
    session,
    &NetClientServerSession::networkPacket,
    handler,
    &ClientServerEventHandler::handleNetworkPacket
  );

  QObject::connect(
    session,
    &NetClientServerSession::connected,
    handler,
    &ClientServerEventHandler::handleConnected
  );

  QObject::connect(
    session,
    &NetClientServerSession::disconnected,
    handler,
    &ClientServerEventHandler::handleDisconnected
  );

  QObject::connect(
    session,
    &NetClientServerSession::error,
    handler,
    &ClientServerEventHandler::handleError
  );

  session->connect();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
