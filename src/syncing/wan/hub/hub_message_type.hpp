#pragma once

#include "nlohmann/json.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
enum class HubMessageType {
  CLIPBOARD_DISPATCH,
  CLIPBOARD_FORWARD,
  DEVICE_ADDED,
  DEVICE_REMOVED,
  DEVICE_UPDATED,
  NONCE_CHALLENGE_COMPLETED,
  NONCE_CHALLENGE_REQUEST,
  NONCE_CHALLENGE_RESPONSE,
  HUB_DEVICES,
};

NLOHMANN_JSON_SERIALIZE_ENUM(HubMessageType, {
  {HubMessageType::CLIPBOARD_DISPATCH,        "CLIPBOARD_DISPATCH"},
  {HubMessageType::CLIPBOARD_FORWARD,         "CLIPBOARD_FORWARD"},
  {HubMessageType::DEVICE_ADDED,              "DEVICE_ADDED"},
  {HubMessageType::DEVICE_REMOVED,            "DEVICE_REMOVED"},
  {HubMessageType::DEVICE_UPDATED,            "DEVICE_UPDATED"},
  {HubMessageType::NONCE_CHALLENGE_COMPLETED, "NONCE_CHALLENGE_COMPLETED"},
  {HubMessageType::NONCE_CHALLENGE_REQUEST,   "NONCE_CHALLENGE_REQUEST"},
  {HubMessageType::NONCE_CHALLENGE_RESPONSE,  "NONCE_CHALLENGE_RESPONSE"},
  {HubMessageType::HUB_DEVICES,               "HUB_DEVICES"},
})
} // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
