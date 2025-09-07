#pragma once

#include <QObject>

#include "hub.hpp"
#include "hub_message.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
struct HubMessagePayloadHandlerBase: public QObject {
 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubMessagePayloadHandlerBase)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubMessagePayloadHandlerBase(QObject* parent = nullptr) : QObject(parent) {}

  /**
   * @brief Destroy the Client object
   */
  virtual ~HubMessagePayloadHandlerBase() = default;

 public:

  /**
   * Dispatch the message to the appropriate handler
   */
  virtual void dispatch(Hub *hub, const HubMessage& message) = 0;
};
}
