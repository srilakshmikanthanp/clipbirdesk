#pragma once

#include <QString>

#include "syncing/wan/device/device_response_dto.hpp"
#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageNonceChallengeCompletedPayload {
  DeviceResponseDto device;
};

template<>
struct HubMessageTypeOf<HubMessageNonceChallengeCompletedPayload> {
  static constexpr HubMessageType value = HubMessageType::NONCE_CHALLENGE_COMPLETED;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageNonceChallengeCompletedPayload,
  device
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
