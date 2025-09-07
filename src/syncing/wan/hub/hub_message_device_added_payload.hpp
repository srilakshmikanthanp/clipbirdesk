#pragma once

#include "syncing/wan/device/device_response_dto.hpp"

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageDeviceAddedPayload {
  DeviceResponseDto device;
};

template<>
struct HubMessageTypeOf<HubMessageDeviceAddedPayload> {
  static constexpr HubMessageType value = HubMessageType::DEVICE_ADDED;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageDeviceAddedPayload,
  device
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
