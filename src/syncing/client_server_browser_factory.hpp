#pragma once

#include "common/trust/trusted_servers_factory.hpp"
#include "common/types/ssl_config/ssl_config_factory.hpp"
#include "syncing/bluetooth/bt_client_server_browser.hpp"
#include "syncing/network/net_client_server_browser.hpp"
#include "client_server_browser.hpp"
#include "application_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
struct ClientServerBrowserFactory {
  static inline ClientServerBrowser* createBluetoothClientServerBrowser(QObject* parent = nullptr) {
    common::trust::TrustedServers* trustedServers = common::trust::TrustedServersFactory::getTrustedServers();
    common::types::SslConfig sslConfig = common::types::SslConfigFactory::getHostSslConfig();
    return new bluetooth::BtClientServerBrowser(sslConfig, trustedServers, parent);
  }

  static inline ClientServerBrowser* createNetworkClientServerBrowser(QObject* parent = nullptr) {
    common::trust::TrustedServers* trustedServers = common::trust::TrustedServersFactory::getTrustedServers();
    common::types::SslConfig sslConfig = common::types::SslConfigFactory::getHostSslConfig();
    return new network::NetClientServerBrowser(sslConfig, trustedServers, parent);
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
