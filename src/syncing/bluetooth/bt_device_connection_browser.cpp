#include "bt_device_connection_browser.hpp"
#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtDeviceConnectionBrowser::handleServiceDiscovered(const QBluetoothServiceInfo &info) {
  if (localDevice.pairingStatus(info.device().address()) != QBluetoothLocalDevice::AuthorizedPaired) {
    return;
  }

  if (localDevice.connectedDevices().indexOf(info.device().address()) == -1) {
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

void BtDeviceConnectionBrowser::handleDeviceConnected() {
  if (!discoveryAgent->isActive()) {
    this->discoveryAgent->start();
  }
}

void BtDeviceConnectionBrowser::handleDeviceDisconnected(const QBluetoothAddress &address) {
  BtResolvedDevice toRemove;
  for (const BtResolvedDevice &device : devices) {
    if (device.address == address) {
      toRemove = device;
      break;
    }
  }

  if (toRemove.address.isNull()) {
    return;
  }

  devices.remove(toRemove);
  emit onServiceRemoved(toRemove);
}

BtDeviceConnectionBrowser::BtDeviceConnectionBrowser(QObject *parent) : BtBrowser(parent) {
  discoveryAgent->setUuidFilter(QBluetoothUuid(BT_SERVICE_UUID));

  QObject::connect(
    discoveryAgent,
    &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
    this,
    &BtDeviceConnectionBrowser::handleServiceDiscovered
  );

  QObject::connect(
    &localDevice,
    &QBluetoothLocalDevice::deviceDisconnected,
    this,
    &BtDeviceConnectionBrowser::handleDeviceDisconnected
  );
}

BtDeviceConnectionBrowser::~BtDeviceConnectionBrowser() {
  this->stop();
}

void BtDeviceConnectionBrowser::start() {
  QObject::connect(&localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BtDeviceConnectionBrowser::handleDeviceConnected);
  discoveryAgent->start();
  emit onBrowsingStarted();
}

void BtDeviceConnectionBrowser::stop() {
  QObject::disconnect(&localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BtDeviceConnectionBrowser::handleDeviceConnected);
  discoveryAgent->stop();
  emit onBrowsingStopped();
}
}
