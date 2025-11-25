#include "bt_sdp_browser.hpp"
#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtSdpBrowser::handleServiceDiscovered(const QBluetoothServiceInfo &info) {
  if (localDevice.pairingStatus(info.device().address()) != QBluetoothLocalDevice::AuthorizedPaired) {
    return;
  }

  BtResolvedDevice resolvedDevice {
    .name = info.device().name(),
    .address = info.device().address(),
    .uuid = info.serviceUuid()
  };

  if (devices.contains(resolvedDevice)) {
    return;
  }

  devices.insert(resolvedDevice);
  emit onServiceAdded(resolvedDevice);
}

void BtSdpBrowser::handleDiscoveryFinished() {
  QTimer::singleShot(BROWSER_INTERVAL_MS, this, [this]() {
    this->discoveryAgent->start();
  });
}

BtSdpBrowser::BtSdpBrowser(QObject *parent) : BtBrowser(parent) {
  discoveryAgent->setUuidFilter(QBluetoothUuid(BT_SERVICE_UUID));

  QObject::connect(
    discoveryAgent,
    &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
    this,
    &BtSdpBrowser::handleServiceDiscovered
  );

  QObject::connect(
    discoveryAgent,
    &QBluetoothServiceDiscoveryAgent::finished,
    this,
    &BtSdpBrowser::handleDiscoveryFinished
  );
}

BtSdpBrowser::~BtSdpBrowser() {
  this->stop();
}

void BtSdpBrowser::start() {
  discoveryAgent->start();
  emit onBrowsingStarted();
}

void BtSdpBrowser::stop() {
  discoveryAgent->stop();
  emit onBrowsingStopped();
}
}
