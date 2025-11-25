#include "client_server_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
ClientServerBrowser::ClientServerBrowser(const common::types::SslConfig& sslConfig, QObject* parent): QObject(parent), sslConfig(sslConfig) {}
ClientServerBrowser::~ClientServerBrowser() {}
}
