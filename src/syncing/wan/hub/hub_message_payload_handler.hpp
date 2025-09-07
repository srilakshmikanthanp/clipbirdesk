#pragma once

#include <QObject>

#include "hub_message_payload_handler_base.hpp"
#include "hub.hpp"
#include "hub_message.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
template <typename Payload>
struct HubMessagePayloadHandler: public HubMessagePayloadHandlerBase {
 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessagePayloadHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessagePayloadHandler(QObject* parent = nullptr) : HubMessagePayloadHandlerBase(parent) {}

  /**
   * @brief Destroy the Client object
   */
  virtual ~HubMessagePayloadHandler() = default;

 public:

  /**
   * @brief Dispatch the message to the appropriate handler
   */
  void dispatch(Hub *hub, const HubMessage& message) override {
    this->handle(hub, std::get<Payload>(message.payload));
  }

 public:

  /**
   * @brief Handle the payload
   */
  virtual void handle(Hub *hub, const Payload& payload) = 0;
};
}
