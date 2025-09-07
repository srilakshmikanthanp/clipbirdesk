#include "hub_message_payload_handler.hpp"
#include "hub_message_nonce_challenge_request_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageNonceChallengeRequestPayloadHandler : public HubMessagePayloadHandler<HubMessageNonceChallengeRequestPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageNonceChallengeRequestPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageNonceChallengeRequestPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageNonceChallengeRequestPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
