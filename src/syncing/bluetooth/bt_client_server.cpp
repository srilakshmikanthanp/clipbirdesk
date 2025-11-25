#include "bt_client_server.hpp"

#include "bt_client_server_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
BtClientServer::BtClientServer(
  const BtResolvedDevice& device,
  const common::types::SslConfig& sslConfig,
  common::trust::TrustedServers* trustedServers,
  QObject* parent
) : ClientServer(device.name, parent),
  btResolvedDevice(device),
  sslConfig(sslConfig),
  trustedServers(trustedServers) {
  // No additional implementation needed
}

BtClientServer::~BtClientServer() {
  // No additional implementation needed
}

void BtClientServer::connect() {
  BtClientServerSession* session = new BtClientServerSession(
    trustedServers,
    btResolvedDevice,
    sslConfig,
    this
  );

  QObject::connect(
    session,
    &BtClientServerSession::networkPacket,
    this,
    &BtClientServer::onNetworkPacket
  );

  QObject::connect(
    session,
    &BtClientServerSession::connected,
    this,
    &BtClientServer::onConnected
  );

  QObject::connect(
    session,
    &BtClientServerSession::disconnected,
    this,
    &BtClientServer::onDisconnected
  );

  QObject::connect(
    session,
    &BtClientServerSession::error,
    this,
    &BtClientServer::onError
  );

  session->connect();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
