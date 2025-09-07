#include "hub_message_devices_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageDevicesPayloadHandler::HubMessageDevicesPayloadHandler(QObject* parent) : HubMessagePayloadHandler<HubMessageDevicesPayload>(parent) {}

void HubMessageDevicesPayloadHandler::handle(Hub *hub, const HubMessageDevicesPayload& payload) {
  for (const auto& device : payload.devices) {
    hub->putHubDevice(device);
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
