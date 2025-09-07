#include "register.hpp"

namespace srilakshmikanthanp::clipbirdesk::mdns {
/**
 * @brief Construct a new Register object
 */
Register::Register(QString serviceName, QString serviceType, QObject *parent)
    : QObject(parent), m_serviceName(std::move(serviceName)), m_serviceType(std::move(serviceType)) {
  // Empty Constructor just initializes the service name and type
}

/**
 * @brief Destructor
 */
Register::~Register() = default;
}  // namespace srilakshmikanthanp::clipbirdesk::network::service::mdns
