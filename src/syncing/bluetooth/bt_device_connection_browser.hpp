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
class BtDeviceConnectionBrowser : public BtBrowser {
 private:

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(BtDeviceConnectionBrowser)

 private:
  QBluetoothServiceDiscoveryAgent *discoveryAgent = new QBluetoothServiceDiscoveryAgent(this);
  QSet<BtResolvedDevice> devices;
  QBluetoothLocalDevice localDevice;
  const qsizetype BROWSER_INTERVAL_MS = 30000;

 private:
  void handleServiceDiscovered(const QBluetoothServiceInfo &info);
  void handleDiscoveryFinished();
  void handleDeviceDisconnected(const QBluetoothAddress &address);

 public:

  BtDeviceConnectionBrowser(QObject *parent = nullptr);
  virtual ~BtDeviceConnectionBrowser();

  void start() override;
  void stop() override;
};
}
