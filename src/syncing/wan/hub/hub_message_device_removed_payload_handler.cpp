#include "hub_message_device_removed_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageDeviceRemovedPayloadHandler::HubMessageDeviceRemovedPayloadHandler(QObject* parent) : HubMessagePayloadHandler<HubMessageDeviceRemovedPayload>(parent) {}

void HubMessageDeviceRemovedPayloadHandler::handle(Hub *hub, const HubMessageDeviceRemovedPayload& payload) {
  hub->putHubDevice(payload.device);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
