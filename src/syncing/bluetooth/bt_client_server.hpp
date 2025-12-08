#pragma once

#include <QObject>
#include <QString>

#include "syncing/client_server.hpp"
#include "syncing/bluetooth/bt_resolved_device.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "common/trust/trusted_servers.hpp"
#include "syncing/client_server_event_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtClientServer : public ClientServer {
 private:
  BtResolvedDevice btResolvedDevice;
  common::types::SslConfig sslConfig;
  common::trust::TrustedServers* trustedServers;

 public:
  explicit BtClientServer(
    const BtResolvedDevice& device,
    const common::types::SslConfig& sslConfig,
    common::trust::TrustedServers* trustedServers,
    QObject* parent = nullptr
  );
  virtual ~BtClientServer();
  virtual void connect(
    syncing::ClientServerEventHandler *handler
  ) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
