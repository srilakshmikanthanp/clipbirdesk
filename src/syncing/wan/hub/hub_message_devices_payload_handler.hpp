#include "hub_message_payload_handler.hpp"
#include "hub_message_devices_payload.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubMessageDevicesPayloadHandler : public HubMessagePayloadHandler<HubMessageDevicesPayload> {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageDevicesPayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageDevicesPayloadHandler(QObject* parent = nullptr);

 public:

  /**
   * @brief handle the message
   */
  void handle(Hub *hub, const HubMessageDevicesPayload& payload) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
