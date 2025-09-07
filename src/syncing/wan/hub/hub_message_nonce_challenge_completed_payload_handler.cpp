#include "hub_message_nonce_challenge_completed_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageNonceChallengeCompletedPayloadHandler::HubMessageNonceChallengeCompletedPayloadHandler(QObject* parent): HubMessagePayloadHandler<HubMessageNonceChallengeCompletedPayload>(parent) {}

void HubMessageNonceChallengeCompletedPayloadHandler::handle(Hub *hub, const HubMessageNonceChallengeCompletedPayload& payload) {
  emit hub->OnConnected();
}
}
