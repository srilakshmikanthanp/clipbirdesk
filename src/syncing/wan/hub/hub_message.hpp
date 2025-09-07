#pragma once

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "syncing/wan/hub/hub_message_type.hpp"
#include "syncing/wan/hub/hub_message_payload_variant.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessage {
  HubMessageType type;
  HubMessagePayloadVariant payload;
};

inline void from_json(const nlohmann::json& j, HubMessage& message) {
  j.at("type").get_to(message.type);
  switch (message.type) {
  case HubMessageType::CLIPBOARD_DISPATCH:
    message.payload = j.at("payload").get<HubMessageClipboardDispatchPayload>();
    break;
  case HubMessageType::CLIPBOARD_FORWARD:
    message.payload = j.at("payload").get<HubMessageClipboardForwardPayload>();
    break;
  case HubMessageType::DEVICE_ADDED:
    message.payload = j.at("payload").get<HubMessageDeviceAddedPayload>();
    break;
  case HubMessageType::DEVICE_REMOVED:
    message.payload = j.at("payload").get<HubMessageDeviceRemovedPayload>();
    break;
  case HubMessageType::DEVICE_UPDATED:
    message.payload = j.at("payload").get<HubMessageDeviceUpdatedPayload>();
    break;
  case HubMessageType::NONCE_CHALLENGE_COMPLETED:
    message.payload = j.at("payload").get<HubMessageNonceChallengeCompletedPayload>();
    break;
  case HubMessageType::NONCE_CHALLENGE_REQUEST:
    message.payload = j.at("payload").get<HubMessageNonceChallengeRequestPayload>();
    break;
  case HubMessageType::NONCE_CHALLENGE_RESPONSE:
    message.payload = j.at("payload").get<HubMessageNonceChallengeResponsePayload>();
    break;
  case HubMessageType::HUB_DEVICES:
    message.payload = j.at("payload").get<HubMessageDevicesPayload>();
    break;
  default:
    throw std::runtime_error("Unknown HubMessageType");
  }
}

inline void to_json(nlohmann::json& j, const HubMessage& message) {
  j = nlohmann::json{ {"type", message.type}, {"payload", std::visit([](const auto& p) { return nlohmann::json(p); }, message.payload)}};
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
