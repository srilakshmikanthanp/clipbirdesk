#include "net_register.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
/**
 * @brief Construct a new Register object
 */
Register::Register(QObject *parent): QObject(parent) {
  // Empty Constructor just initializes the service name and type
}

/**
 * @brief Destructor
 */
Register::~Register() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::mdns
