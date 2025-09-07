#pragma once

#include "syncing/wan/device/device_response_dto.hpp"
#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageDeviceRemovedPayload {
  DeviceResponseDto device;
};

template<>
struct HubMessageTypeOf<HubMessageDeviceRemovedPayload> {
  static constexpr HubMessageType value = HubMessageType::DEVICE_REMOVED;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageDeviceRemovedPayload,
  device
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
