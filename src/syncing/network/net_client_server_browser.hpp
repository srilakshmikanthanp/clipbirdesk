#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include "syncing/client_server_browser.hpp"
#include "syncing/network/net_browser.hpp"
#include "syncing/network/net_client_server.hpp"
#include "syncing/network/net_resolved_device.hpp"
#include "syncing/network/net_mdns.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "common/trust/trusted_servers.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {

class NetClientServerBrowser : public ClientServerBrowser {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(NetClientServerBrowser)

 private:
  MdnsBrowser* mdnsBrowser;
  common::trust::TrustedServers* trustedServers;
  QMap<NetResolvedDevice, ClientServer*> clientServers;

 private:
  // mDNS browser event handlers
  void handleBrowsingStartFailed(std::exception_ptr eptr);
  void handleBrowsingStopFailed(std::exception_ptr eptr);
  void handleBrowsingStarted();
  void handleBrowsingStopped();
  void handleServiceAdded(NetResolvedDevice device);
  void handleServiceRemoved(NetResolvedDevice device);

 public:
  explicit NetClientServerBrowser(
    const common::types::SslConfig& sslConfig,
    common::trust::TrustedServers* trustedServers,
    QObject* parent = nullptr
  );

  virtual ~NetClientServerBrowser();

  virtual void start() override;
  virtual void stop() override;
};

}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
