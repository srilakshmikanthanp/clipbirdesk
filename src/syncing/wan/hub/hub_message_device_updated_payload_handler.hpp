#include "hub_message_payload_handler.hpp"
#include "hub_message_device_updated_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageDeviceUpdatedPayloadHandler : public HubMessagePayloadHandler<HubMessageDeviceUpdatedPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageDeviceUpdatedPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageDeviceUpdatedPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageDeviceUpdatedPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
