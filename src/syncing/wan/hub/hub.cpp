#include "hub.hpp"

#include "hub_message_clipboard_forward_payload.hpp"
#include "hub_websocket.hpp"
#include "utility/functions/crypto/crypto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 * @brief Construct a new Hub object
 */
Hub::Hub(HubHostDevice hubHostDevice, QObject* parent): Synchronizer(parent), hubHostDevice(hubHostDevice) {}

/**
 * @brief Destroy the Hub object
 */
Hub::~Hub() = default;

/**
 * @brief Synchronize the clipboard data
 */
void Hub::synchronize(const QVector<QPair<QString, QByteArray>>& items) {
  for (const auto& hubDevice : this->getHubDevices()) {
    HubMessageClipboardForwardPayload payload{.toDeviceId = hubDevice.id, .clipboard = items};
    for (auto& item : payload.clipboard) item.second = utility::functions::encrypt(item.second, hubDevice.publicKey.toUtf8());
    this->sendMessage(QString::fromUtf8(nlohmann::json(payload).dump()));
  }
}

/**
 * @brief get Hub host device
 */
HubHostDevice Hub::getHubHostDevice() const {
  return this->hubHostDevice;
}

/**
 * @brief get Hub devices
 */
std::unordered_set<DeviceResponseDto> Hub::getHubDevices() {
  return this->hubDevices;
}

/**
 * @brief remove device
 */
void Hub::removeHubDevice(const DeviceResponseDto& device) {
  this->hubDevices.erase(device);
}

/**
 * @brief put device
 */
void Hub::putHubDevice(const DeviceResponseDto& device) {
  this->hubDevices.insert(device);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
