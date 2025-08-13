#include "browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::mdns {
/**
 * Constructs a new Browser object
 */
Browser::Browser(QString serviceName, QString serviceType, QObject *parent)
    : QObject(parent), m_serviceName(std::move(serviceName)), m_serviceType(std::move(serviceType)) {
}

/**
 * Destroys the Browser object
 */
Browser::~Browser() = default;
}
