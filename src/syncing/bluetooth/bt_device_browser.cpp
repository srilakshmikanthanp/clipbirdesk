#include "bt_device_browser.hpp"
#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtDeviceBrowser::handleServiceDiscovered(const QBluetoothServiceInfo &info) {
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

void BtDeviceBrowser::startDiscovery() {
  if (!discoveryAgent->isActive()) {
    this->discoveryAgent->start();
  }
}

BtDeviceBrowser::BtDeviceBrowser(QObject *parent) : BtBrowser(parent) {
  discoveryAgent->setUuidFilter(QBluetoothUuid(BT_SERVICE_UUID));

  QObject::connect(
    discoveryAgent,
    &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
    this,
    &BtDeviceBrowser::handleServiceDiscovered
  );
}

BtDeviceBrowser::~BtDeviceBrowser() {
  this->stop();
}

void BtDeviceBrowser::start() {
  QObject::connect(&localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BtDeviceBrowser::startDiscovery);
  discoveryAgent->start();
  emit onBrowsingStarted();
}

void BtDeviceBrowser::stop() {
  QObject::disconnect(&localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BtDeviceBrowser::startDiscovery);
  discoveryAgent->stop();
  emit onBrowsingStopped();
}
}
