#pragma once

#include <QObject>
#include <QMap>

#include "hub_message_payload_handler_base.hpp"
#include "hub_message_payload_handler.hpp"
#include "hub.hpp"
#include "hub_message.hpp"
#include "hub_message_type.hpp"
#include "hub_message_type_of.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessageHandler : public QObject {
 private:

  QMap<HubMessageType, HubMessagePayloadHandlerBase*> payloadHandlers;

 private:

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessageHandler)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessageHandler(QObject* parent = nullptr);

  /**
   * @brief Destroy the Client object
   */
  virtual ~HubMessageHandler();

 public:

  template<typename Payload>
  void setPayloadHandler(HubMessagePayloadHandler<Payload>* handler) {
    payloadHandlers[HubMessageTypeOf<Payload>::value] = handler;
  }

 public:

  /**
   * @brief handle the message
   */
  virtual void handle(Hub* hub, const HubMessage& message);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
