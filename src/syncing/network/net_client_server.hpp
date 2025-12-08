#pragma once

#include <QObject>
#include <QString>

#include "syncing/client_server.hpp"
#include "syncing/network/net_resolved_device.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "common/trust/trusted_servers.hpp"
#include "syncing/client_server_event_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
class NetClientServer : public ClientServer {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(NetClientServer)

 private:
  NetResolvedDevice netResolvedDevice;
  common::types::SslConfig sslConfig;
  common::trust::TrustedServers* trustedServers;

 public:
  explicit NetClientServer(
    const NetResolvedDevice& device,
    const common::types::SslConfig& sslConfig,
    common::trust::TrustedServers* trustedServers,
    QObject* parent = nullptr
  );
  virtual ~NetClientServer();
  virtual void connect(
    syncing::ClientServerEventHandler *handler
  ) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
