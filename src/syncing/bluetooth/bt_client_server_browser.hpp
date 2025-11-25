#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include "syncing/client_server_browser.hpp"
#include "syncing/bluetooth/bt_browser.hpp"
#include "syncing/bluetooth/bt_client_server.hpp"
#include "syncing/bluetooth/bt_resolved_device.hpp"
#include "syncing/bluetooth/bt_sdp_browser.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "common/trust/trusted_servers.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {

class BtClientServerBrowser : public ClientServerBrowser {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(BtClientServerBrowser)

 private:
  BtSdpBrowser *sdpBrowser = new BtSdpBrowser(this);
  common::trust::TrustedServers* trustedServers;
  QMap<BtResolvedDevice, ClientServer*> clientServers;

 private:
  // mDNS browser event handlers
  void handleBrowsingStartFailed(std::exception_ptr eptr);
  void handleBrowsingStopFailed(std::exception_ptr eptr);
  void handleBrowsingStarted();
  void handleBrowsingStopped();
  void handleServiceAdded(BtResolvedDevice device);
  void handleServiceRemoved(BtResolvedDevice device);

 public:
  explicit BtClientServerBrowser(
    const common::types::SslConfig& sslConfig,
    common::trust::TrustedServers* trustedServers,
    QObject* parent = nullptr
  );

  virtual ~BtClientServerBrowser();

  virtual void start() override;
  virtual void stop() override;
};

}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
