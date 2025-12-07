#include "bt_device_connection_browser.hpp"
#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtDeviceConnectionBrowser::handleServiceDiscovered(const QBluetoothServiceInfo &info) {
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

void BtDeviceConnectionBrowser::handleDiscoveryFinished() {
  QTimer::singleShot(BROWSER_INTERVAL_MS, this, [this]() {
    this->discoveryAgent->start();
  });
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
    discoveryAgent,
    &QBluetoothServiceDiscoveryAgent::finished,
    this,
    &BtDeviceConnectionBrowser::handleDiscoveryFinished
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
  discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
  emit onBrowsingStarted();
}

void BtDeviceConnectionBrowser::stop() {
  discoveryAgent->stop();
  emit onBrowsingStopped();
}
}
