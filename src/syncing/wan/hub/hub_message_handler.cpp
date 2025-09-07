#include "hub_message_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
HubMessageHandler::HubMessageHandler(QObject* parent) : QObject(parent) {}

HubMessageHandler::~HubMessageHandler() = default;

void HubMessageHandler::handle(Hub *hub, const HubMessage& message) {
  if (payloadHandlers.contains(message.type)) {
    payloadHandlers[message.type]->dispatch(hub, message);
  } else {
    throw std::runtime_error("No handler for message type");
  }
}
}
