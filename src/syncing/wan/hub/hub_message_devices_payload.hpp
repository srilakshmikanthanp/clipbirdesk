#pragma once

#include <vector>

#include "syncing/wan/device/device_response_dto.hpp"
#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageDevicesPayload {
  std::vector<DeviceResponseDto> devices;
};

template<>
struct HubMessageTypeOf<HubMessageDevicesPayload> {
  static constexpr HubMessageType value = HubMessageType::HUB_DEVICES;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageDevicesPayload,
  devices
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
