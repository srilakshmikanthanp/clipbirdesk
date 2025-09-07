#include "hub_message_device_added_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageDeviceAddedPayloadHandler::HubMessageDeviceAddedPayloadHandler(QObject* parent) : HubMessagePayloadHandler<HubMessageDeviceAddedPayload>(parent) {}

void HubMessageDeviceAddedPayloadHandler::handle(Hub *hub, const HubMessageDeviceAddedPayload& payload) {
  hub->putHubDevice(payload.device);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
