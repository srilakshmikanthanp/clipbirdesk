#pragma once

#include <variant>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"

#include "hub_message_clipboard_dispatch_payload.hpp"
#include "hub_message_clipboard_forward_payload.hpp"
#include "hub_message_device_added_payload.hpp"
#include "hub_message_device_removed_payload.hpp"
#include "hub_message_device_updated_payload.hpp"
#include "hub_message_devices_payload.hpp"
#include "hub_message_nonce_challenge_completed_payload.hpp"
#include "hub_message_nonce_challenge_request_payload.hpp"
#include "hub_message_nonce_challenge_response_payload.hpp"
#include "hub_message_type.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
using HubMessagePayloadVariant = std::variant<
  HubMessageClipboardDispatchPayload,
  HubMessageClipboardForwardPayload,
  HubMessageDeviceAddedPayload,
  HubMessageDeviceRemovedPayload,
  HubMessageDeviceUpdatedPayload,
  HubMessageDevicesPayload,
  HubMessageNonceChallengeCompletedPayload,
  HubMessageNonceChallengeRequestPayload,
  HubMessageNonceChallengeResponsePayload
>;
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
