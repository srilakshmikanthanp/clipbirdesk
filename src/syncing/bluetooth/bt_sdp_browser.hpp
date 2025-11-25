#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>

#include "common/trust/trusted_servers.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "bt_browser.hpp"
#include "bt_resolved_device.hpp"
#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtSdpBrowser : public BtBrowser {
 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(BtSdpBrowser)

 private:
  QBluetoothServiceDiscoveryAgent *discoveryAgent = new QBluetoothServiceDiscoveryAgent(this);
  QSet<BtResolvedDevice> devices;
  QBluetoothLocalDevice localDevice;
  const qsizetype BROWSER_INTERVAL_MS = 30000;

 private:
  void handleServiceDiscovered(const QBluetoothServiceInfo &info);
  void handleDiscoveryFinished();

 public:

  BtSdpBrowser(QObject *parent = nullptr);
  virtual ~BtSdpBrowser();

  void start() override;
  void stop() override;
};
}
