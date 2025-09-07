#include "hub_message_payload_handler.hpp"
#include "hub_message_device_removed_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageDeviceRemovedPayloadHandler : public HubMessagePayloadHandler<HubMessageDeviceRemovedPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageDeviceRemovedPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageDeviceRemovedPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageDeviceRemovedPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
