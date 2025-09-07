#include "hub_message_payload_handler.hpp"
#include "hub_message_clipboard_dispatch_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageClipboardDispatchPayloadHandler : public HubMessagePayloadHandler<HubMessageClipboardDispatchPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageClipboardDispatchPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageClipboardDispatchPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageClipboardDispatchPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
