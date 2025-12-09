#include "clipbird_service.hpp"

namespace srilakshmikanthanp::clipbirdesk::service {
void ClipbirdService::handleClientDisconnected(syncing::Session* session) {
  for (auto server: syncingManager->getAvailableServers()) {
    if (trustedServers->hasTrustedServer(server->getName()) && server->getName() != session->getName()) {
      syncingManager->connectToServer(server);
      break;
    }
  }
}

void ClipbirdService::handleServerFound(syncing::ClientServer* server) {
  if (trustedServers->hasTrustedServer(server->getName()) && syncingManager->getConnectedServer() == nullptr) {
    syncingManager->connectToServer(server);
  }
}

void ClipbirdService::handleClientConnected(syncing::Session* client) {
  if (this->trustedClients->isTrustedClient(client->getName(), client->getCertificate())) {
    client->sendPacket(utility::functions::createPacket(utility::functions::params::AuthenticationParams{common::types::enums::AuthOkay}));
    return;
  }

  ui::gui::notification::JoinRequest *joinRequest = new ui::gui::notification::JoinRequest(this);

  auto handleAccept = [client, joinRequest, this]() {
    trustedClients->addTrustedClient(client->getName(), client->getCertificate());
    client->sendPacket(utility::functions::createPacket(utility::functions::params::AuthenticationParams{common::types::enums::AuthOkay}));
    joinRequest->deleteLater();
  };

  auto handleReject = [client, joinRequest]() {
    client->sendPacket(utility::functions::createPacket(utility::functions::params::AuthenticationParams{common::types::enums::AuthFail}));
    joinRequest->deleteLater();
  };

  QObject::connect(
    joinRequest,
    &ui::gui::notification::JoinRequest::onAccept,
    handleAccept
  );

  QObject::connect(
    joinRequest,
    &ui::gui::notification::JoinRequest::onReject,
    handleReject
  );

  joinRequest->show(client->getName());
}

void ClipbirdService::setHostState(bool isServer, bool useBluetooth) {
  if (isServer) {
    syncingManager->setHostAsServer(useBluetooth);
  } else {
    syncingManager->setHostAsClient(useBluetooth);
  }
}

ClipbirdService::ClipbirdService(QObject* parent): QObject(parent) {
  QObject::connect(
    syncingManager,
    &syncing::SyncingManager::clientDisconnectedEvent,
    this,
    &ClipbirdService::handleClientDisconnected
  );

  QObject::connect(
    syncingManager,
    &syncing::SyncingManager::OnSyncRequest,
    clipboardHistory,
    &history::ClipboardHistory::addHistory
  );

  QObject::connect(
    clipboardHistory,
    &history::ClipboardHistory::onClipboard,
    applicationClipboard,
    &clipboard::ApplicationClipboard::set
  );

  QObject::connect(
    syncingManager,
    &syncing::SyncingManager::serverFoundEvent,
    this,
    &ClipbirdService::handleServerFound
  );

  QObject::connect(
    syncingManager,
    &syncing::SyncingManager::clientConnectedEvent,
    this,
    &ClipbirdService::handleClientConnected
  );

  QObject::connect(
    applicationClipboard,
    &clipboard::ApplicationClipboard::OnClipboardChange,
    syncingManager,
    &syncing::SyncingManager::synchronize
  );

  QObject::connect(
    applicationState,
    &ApplicatiionState::onIsServerChanged,
    [this](bool isServer) {
      setHostState(isServer, applicationState->shouldUseBluetooth());
    }
  );

  QObject::connect(
    applicationState,
    &ApplicatiionState::shouldUseBluetoothChanged,
    [this](bool useBluetooth) {
      setHostState(applicationState->getIsServer(), useBluetooth);
    }
  );

  this->setHostState(
    applicationState->getIsServer(),
    applicationState->shouldUseBluetooth()
  );
}

ClipbirdService::~ClipbirdService() = default;

clipboard::ApplicationClipboard* ClipbirdService::getClipboardManager() {
  return applicationClipboard;
}

history::ClipboardHistory* ClipbirdService::getClipboardHistory() {
  return clipboardHistory;
}

syncing::SyncingManager* ClipbirdService::getSyncingManager() {
  return syncingManager;
}

common::trust::TrustedClients* ClipbirdService::getTrustedClients() {
  return trustedClients;
}

common::trust::TrustedServers* ClipbirdService::getTrustedServers() {
  return trustedServers;
}
}
