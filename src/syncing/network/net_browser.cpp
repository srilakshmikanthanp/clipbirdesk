#include "net_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
/**
 * Constructs a new Browser object
 */
NetBrowser::NetBrowser(QObject *parent): QObject(parent) {
}

/**
 * Destroys the Browser object
 */
NetBrowser::~NetBrowser() = default;
}
