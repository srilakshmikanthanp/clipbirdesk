#include "hub_message_device_updated_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageDeviceUpdatedPayloadHandler::HubMessageDeviceUpdatedPayloadHandler(QObject* parent) : HubMessagePayloadHandler<HubMessageDeviceUpdatedPayload>(parent) {}

void HubMessageDeviceUpdatedPayloadHandler::handle(Hub *hub, const HubMessageDeviceUpdatedPayload& payload) {
  hub->putHubDevice(payload.device);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
