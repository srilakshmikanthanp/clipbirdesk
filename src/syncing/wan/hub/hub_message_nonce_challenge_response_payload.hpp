#pragma once

#include <QString>

#include "common/nlohmann/adl_serializer.hpp"
#include "nlohmann/json.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageNonceChallengeResponsePayload {
  QString signature;
  QString nonce;
};

template<>
struct HubMessageTypeOf<HubMessageNonceChallengeResponsePayload> {
  static constexpr HubMessageType value = HubMessageType::NONCE_CHALLENGE_RESPONSE;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(HubMessageNonceChallengeResponsePayload,
  signature,
  nonce
);
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
