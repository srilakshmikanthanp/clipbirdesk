#include "hub_message_payload_handler.hpp"
#include "hub_message_device_added_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageDeviceAddedPayloadHandler : public HubMessagePayloadHandler<HubMessageDeviceAddedPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageDeviceAddedPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageDeviceAddedPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageDeviceAddedPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
