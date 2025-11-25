#pragma once

#include "common/trust/trusted_clients_factory.hpp"
#include "common/types/ssl_config/ssl_config_factory.hpp"
#include "syncing/bluetooth/bt_server.hpp"
#include "syncing/network/net_server.hpp"
#include "server.hpp"
#include "application_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
struct ServerFactory {
  static inline Server* createBluetoothServer(QObject* parent = nullptr) {
    common::trust::TrustedClients* trustedClients = common::trust::TrustedClientsFactory::getTrustedClients();
    common::types::SslConfig sslConfig = common::types::SslConfigFactory::getHostSslConfig();
    return new bluetooth::BtServer(sslConfig, trustedClients, parent);
  }

  static inline Server* createNetworkServer(QObject* parent = nullptr) {
    common::trust::TrustedClients* trustedClients = common::trust::TrustedClientsFactory::getTrustedClients();
    common::types::SslConfig sslConfig = common::types::SslConfigFactory::getHostSslConfig();
    return new network::NetServer(sslConfig, trustedClients, parent);
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
