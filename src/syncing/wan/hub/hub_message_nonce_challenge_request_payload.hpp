#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageNonceChallengeRequestPayload {
  QString nonce;
};

template<>
struct HubMessageTypeOf<HubMessageNonceChallengeRequestPayload> {
  static constexpr HubMessageType value = HubMessageType::NONCE_CHALLENGE_REQUEST;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageNonceChallengeRequestPayload,
  nonce
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
