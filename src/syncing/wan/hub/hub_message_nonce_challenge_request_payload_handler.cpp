#include "hub_message_nonce_challenge_request_payload_handler.hpp"
#include "hub_message_nonce_challenge_response_payload.hpp"
#include "utility/functions/crypto/crypto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageNonceChallengeRequestPayloadHandler::HubMessageNonceChallengeRequestPayloadHandler(QObject* parent)
  : HubMessagePayloadHandler<HubMessageNonceChallengeRequestPayload>(parent) {}

void HubMessageNonceChallengeRequestPayloadHandler::handle(Hub *hub, const HubMessageNonceChallengeRequestPayload& payload) {
  HubMessageNonceChallengeResponsePayload response { .signature = utility::functions::sign(payload.nonce.toUtf8(), hub->getHubHostDevice().privateKey.toUtf8()).toBase64(), .nonce = payload.nonce };
  HubMessage message { .type = HubMessageType::NONCE_CHALLENGE_RESPONSE, .payload = response };
  hub->sendMessage(nlohmann::json(message).dump().c_str());
}
}
