#include "bt_client_server_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {

BtClientServerBrowser::BtClientServerBrowser(
  const common::types::SslConfig& sslConfig,
  common::trust::TrustedServers* trustedServers,
  QObject* parent
) : ClientServerBrowser(sslConfig, parent),
    trustedServers(trustedServers) {

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onBrowsingStarted,
    this,
    &BtClientServerBrowser::handleBrowsingStarted
  );

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onBrowsingStopped,
    this,
    &BtClientServerBrowser::handleBrowsingStopped
  );

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onServiceAdded,
    this,
    &BtClientServerBrowser::handleServiceAdded
  );

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onServiceRemoved,
    this,
    &BtClientServerBrowser::handleServiceRemoved
  );

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onBrowsingStartFailed,
    this,
    &BtClientServerBrowser::handleBrowsingStartFailed
  );

  QObject::connect(
    sdpBrowser,
    &BtSdpBrowser::onBrowsingStopFailed,
    this,
    &BtClientServerBrowser::handleBrowsingStopFailed
  );
}

void BtClientServerBrowser::handleServiceAdded(BtResolvedDevice device) {
  if (clientServers.contains(device)) return;
  auto clientServer = new BtClientServer(device, sslConfig, trustedServers, this);
  clientServers.insert(device, clientServer);
  emit onServerFound(clientServer);
}

void BtClientServerBrowser::handleServiceRemoved(BtResolvedDevice device) {
  if (!clientServers.contains(device)) return;
  ClientServer* clientServer = clientServers.take(device);
  emit onServerGone(clientServer);
  clientServer->deleteLater();
}

void BtClientServerBrowser::handleBrowsingStarted() {
  emit onBrowsingStarted();
}

void BtClientServerBrowser::handleBrowsingStopped() {
  emit onBrowsingStopped();
}

void BtClientServerBrowser::handleBrowsingStartFailed(std::exception_ptr eptr) {
  emit onBrowsingStartFailed(eptr);
}

void BtClientServerBrowser::handleBrowsingStopFailed(std::exception_ptr eptr) {
  emit onBrowsingStopFailed(eptr);
}

void BtClientServerBrowser::start() {
  sdpBrowser->start();
}

void BtClientServerBrowser::stop() {
  sdpBrowser->stop();
}

BtClientServerBrowser::~BtClientServerBrowser() {
  this->stop();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
