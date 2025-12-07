#pragma once

#include "clipboard/application_clipboard_factory.hpp"
#include "clipboard/applicationclipboard.hpp"
#include "history/clipboard_history_factory.hpp"
#include "history/clipboard_history.hpp"
#include "syncing/manager/syncing_manager_factory.hpp"
#include "syncing/manager/syncing_manager.hpp"
#include "common/trust/trusted_clients.hpp"
#include "common/trust/trusted_servers.hpp"
#include "common/trust/trusted_clients_factory.hpp"
#include "common/trust/trusted_servers_factory.hpp"
#include "ui/gui/notification/joinrequest/joinrequest.hpp"
#include "utility/powerhandler/powerhandler.hpp"

namespace srilakshmikanthanp::clipbirdesk::service {
class ClipbirdService : public QObject {
  Q_OBJECT
 private:
  Q_DISABLE_COPY_MOVE(ClipbirdService)

 private:
  void handleClientDisconnected(syncing::Session* session);
  void handleServerFound(syncing::ClientServer* server);
  void handleClientConnected(syncing::Session* client);
  void setHostState(bool isServer, bool useBluetooth);

 public:
  explicit ClipbirdService(QObject* parent = nullptr);
  ~ClipbirdService() override;

  clipboard::ApplicationClipboard* getClipboardManager();
  history::ClipboardHistory* getClipboardHistory();
  syncing::SyncingManager* getSyncingManager();
  common::trust::TrustedClients* getTrustedClients();
  common::trust::TrustedServers* getTrustedServers();

 private:
  clipboard::ApplicationClipboard *applicationClipboard = clipboard::ApplicationClipboardFactory::getApplicationClipboard();
  history::ClipboardHistory *clipboardHistory = history::ClipboardHistoryFactory::getClipboardHistory();
  syncing::SyncingManager *syncingManager = syncing::SyncingManagerFactory::getSyncingManager();
  common::trust::TrustedClients *trustedClients = common::trust::TrustedClientsFactory::getTrustedClients();
  common::trust::TrustedServers *trustedServers = common::trust::TrustedServersFactory::getTrustedServers();
  ApplicatiionState *applicationState = ApplicationFactory::getApplicationState();
};
}
