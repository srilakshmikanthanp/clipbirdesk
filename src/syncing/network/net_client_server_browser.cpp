#include "net_client_server_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {

NetClientServerBrowser::NetClientServerBrowser(
  const common::types::SslConfig& sslConfig,
  common::trust::TrustedServers* trustedServers,
  QObject* parent
) : ClientServerBrowser(sslConfig, parent),
    trustedServers(trustedServers),
    mdnsBrowser(new MdnsBrowser(this)) {

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onBrowsingStarted,
    this,
    &NetClientServerBrowser::handleBrowsingStarted
  );

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onBrowsingStopped,
    this,
    &NetClientServerBrowser::handleBrowsingStopped
  );

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onServiceAdded,
    this,
    &NetClientServerBrowser::handleServiceAdded
  );

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onServiceRemoved,
    this,
    &NetClientServerBrowser::handleServiceRemoved
  );

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onBrowsingStartFailed,
    this,
    &NetClientServerBrowser::handleBrowsingStartFailed
  );

  QObject::connect(
    mdnsBrowser,
    &MdnsBrowser::onBrowsingStopFailed,
    this,
    &NetClientServerBrowser::handleBrowsingStopFailed
  );
}

void NetClientServerBrowser::handleServiceAdded(NetResolvedDevice device) {
  if (clientServers.contains(device)) return;
  auto clientServer = new NetClientServer(device, sslConfig, trustedServers, this);
  clientServers.insert(device, clientServer);
  emit onServerFound(clientServer);
}

void NetClientServerBrowser::handleServiceRemoved(NetResolvedDevice device) {
  if (!clientServers.contains(device)) return;
  ClientServer* clientServer = clientServers.take(device);
  emit onServerGone(clientServer);
  clientServer->deleteLater();
}

void NetClientServerBrowser::handleBrowsingStarted() {
  emit onBrowsingStarted();
}

void NetClientServerBrowser::handleBrowsingStopped() {
  emit onBrowsingStopped();
}

void NetClientServerBrowser::handleBrowsingStartFailed(std::exception_ptr eptr) {
  emit onBrowsingStartFailed(eptr);
}

void NetClientServerBrowser::handleBrowsingStopFailed(std::exception_ptr eptr) {
  emit onBrowsingStopFailed(eptr);
}

void NetClientServerBrowser::start() {
  mdnsBrowser->start();
}

void NetClientServerBrowser::stop() {
  mdnsBrowser->stop();
}

NetClientServerBrowser::~NetClientServerBrowser() {
  this->stop();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
