#include "wan_service.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
WanService::WanService(syncing::wan::WanController* controller, QObject* parent): QObject(parent), wanController(controller) {
  QObject::connect(reconnectTimer, &QTimer::timeout, this, &WanService::connectToHub);
  reconnectTimer->setSingleShot(true);
}

syncing::wan::WanController* WanService::getWanController() const {
  return wanController;
}

WanService::~WanService() = default;

QFuture<void> WanService::connectToHub() {
 const auto updateHubHostDevice = [=, this](const syncing::wan::HubHostDevice& device) {
    auto requestDto = syncing::wan::DeviceRequestDto{ device.publicKey, device.name, device.type };
    return deviceRepository->updateDevice(device.id, requestDto).then([=, this](const syncing::wan::DeviceResponseDto& updatedDevice) {
      return syncing::wan::HubHostDevice{updatedDevice.id, updatedDevice.name, updatedDevice.type, updatedDevice.publicKey, device.privateKey};
    });
  };

  const auto createHubHostDevice = [=, this]() {
    auto [privateKey, publicKey] = utility::functions::generateQtKeyPair();
    auto requestDto = syncing::wan::DeviceRequestDto{ publicKey.toPem(), constants::getMDnsServiceName(), syncing::wan::DeviceType::getCurrentDeviceType() };
    return deviceRepository->createDevice(requestDto).then([=, this](const syncing::wan::DeviceResponseDto& device) {
      return syncing::wan::HubHostDevice{device.id, device.name, device.type, device.publicKey, privateKey.toPem()};
    });
  };

  const auto saveHubHostDevice = [=](const syncing::wan::HubHostDevice& device) {
    QKeychain::WritePasswordJob *job = nullptr;
    bool status = QMetaObject::invokeMethod(
      &storage::SecureStorage::instance(),
      &storage::SecureStorage::setHubHostDevice,
      Qt::BlockingQueuedConnection,
      qReturnArg(job),
      device
    );
    assert(status && job != nullptr);
    return utility::functions::toFuture(job).then([device]() {
      QPromise<syncing::wan::HubHostDevice> promise;
      promise.addResult(device);
      promise.finish();
      return promise.future();
    }).unwrap();
  };

  storage::Storage::instance().setIsConnectedToHubLastly(true);
  this->resetReconnectSchedule();
  emit this->wanController->onConnecting();

  return utility::functions::toFuture(
    storage::SecureStorage::instance().getHubHostDevice(),
    &storage::ReadHubHostDeviceJob::device
  ).then([=, this](std::optional<syncing::wan::HubHostDevice> device) {
    if (device.has_value()) {
      syncing::wan::HubHostDevice hubDevice = device.value();
      hubDevice.name = constants::getMDnsServiceName();
      return updateHubHostDevice(hubDevice);
    } else {
      return createHubHostDevice();
    }
  })
  .unwrap()
  .then([=, this](const syncing::wan::HubHostDevice& device) {
    return saveHubHostDevice(device);
  })
  .unwrap()
  .then([=, this](const syncing::wan::HubHostDevice& device) {
    wanController->connectToHub(device);
  });
}

void WanService::scheduleReconnect() {
  if (reconnectTimer->isActive() || this->wanController->isHubOpen()) return;
  int delay = std::min(static_cast<int>(baseDelayMs * std::pow(backOffFactor, reconnectAttempts)), maxDelayMs);
  reconnectTimer->start(delay);
  reconnectAttempts++;
}

void WanService::resetReconnectSchedule() {
  reconnectAttempts = 0;
  reconnectTimer->stop();
}

void WanService::disconnectFromHub() {
  storage::Storage::instance().setIsConnectedToHubLastly(false);
  this->wanController->disconnectFromHub();
  this->resetReconnectSchedule();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
