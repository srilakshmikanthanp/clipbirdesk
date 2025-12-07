#include "clipbird_qml_syncing_manager.hpp"

#include "ui/gui/binding/clipboard/clipbird_qml_application_clipboard.hpp"
#include "syncing/manager/syncing_manager_factory.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

void ClipbirdQmlSyncingManager::putConnectedClients(QVector<syncing::Session*> clients) {
  for (auto* client : m_connectedClients) {
    client->deleteLater();
  }
  m_connectedClients.clear();
  for (auto* client : clients) {
    m_connectedClients.append(new ClipbirdQmlSession(client, this));
  }
  emit connectedClientsChanged();
}

void ClipbirdQmlSyncingManager::putConnectedServer(syncing::Session* server) {
  if (m_connectedServer) {
    m_connectedServer->deleteLater();
    m_connectedServer = nullptr;
  }
  if (server) {
    m_connectedServer = new ClipbirdQmlSession(server, this);
  }
  emit connectedServerChanged();
}

void ClipbirdQmlSyncingManager::putAvailableServers(QVector<syncing::ClientServer*> servers) {
  for (auto* server : m_availableServers) {
    server->deleteLater();
  }
  m_availableServers.clear();
  for (auto* server : servers) {
    m_availableServers.append(new ClipbirdQmlClientServer(server, this));
  }
  emit availableServersChanged();
}

ClipbirdQmlSyncingManager::ClipbirdQmlSyncingManager(syncing::SyncingManager* syncingManager, QObject* parent)
    : QObject(parent), m_syncingManager(syncingManager) {
  connect(
    m_syncingManager,
    &syncing::SyncingManager::connectedClientsChanged,
    this,
    &ClipbirdQmlSyncingManager::putConnectedClients
  );

  connect(
    m_syncingManager,
    &syncing::SyncingManager::connectedServerChanged,
    this,
    &ClipbirdQmlSyncingManager::putConnectedServer
  );

  connect(
    m_syncingManager,
    &syncing::SyncingManager::availableServersChanged,
    this,
    &ClipbirdQmlSyncingManager::putAvailableServers
  );

  putConnectedClients(m_syncingManager->getConnectedClients());
  putConnectedServer(m_syncingManager->getConnectedServer());
  putAvailableServers(m_syncingManager->getAvailableServers());
}

ClipbirdQmlSyncingManager::~ClipbirdQmlSyncingManager() {
  if (m_connectedServer) {
    m_connectedServer->deleteLater();
    m_connectedServer = nullptr;
  }
  for (auto* client : m_connectedClients) {
    client->deleteLater();
  }
  m_connectedClients.clear();
  for (auto* server : m_availableServers) {
    server->deleteLater();
  }
  m_availableServers.clear();
}

QList<ClipbirdQmlSession*> ClipbirdQmlSyncingManager::getConnectedClients() const {
  return m_connectedClients;
}

ClipbirdQmlSession* ClipbirdQmlSyncingManager::getConnectedServer() const {
  return m_connectedServer;
}

QList<ClipbirdQmlClientServer*> ClipbirdQmlSyncingManager::getAvailableServers() const {
  return m_availableServers;
}

void ClipbirdQmlSyncingManager::connectToServer(const QString& serverName) {
  auto server = m_syncingManager->getClientServerByName(serverName);
  if (!server.has_value()) throw std::runtime_error("Server with name " + serverName.toStdString() + " not found");
  m_syncingManager->connectToServer(server.value());
}

void ClipbirdQmlSyncingManager::disconnectFromServer() {
  auto* server = m_syncingManager->getConnectedServer();
  if (!server) throw std::runtime_error("No connected server to disconnect from");
  server->disconnectFromHost();
}

void ClipbirdQmlSyncingManager::disconnectClient(const QString& clientName) {
  auto client = m_syncingManager->getServerClientSessionByName(clientName);
  if (!client.has_value()) throw std::runtime_error("Client with name " + clientName.toStdString() + " not found");
  client.value()->disconnectFromHost();
}

void ClipbirdQmlSyncingManager::setHostAsServer(bool useBluetooth) {
  m_syncingManager->setHostAsServer(useBluetooth);
}

void ClipbirdQmlSyncingManager::setHostAsClient(bool useBluetooth) {
  m_syncingManager->setHostAsClient(useBluetooth);
}

void ClipbirdQmlSyncingManager::synchronize(const QVariantList& items) {
  QVector<QPair<QString, QByteArray>> clipboardData;
  for (const auto& item : items) {
    QVariantMap itemMap = item.toMap();
    QString mimeType = itemMap[ClipbirdQmlApplicationClipboard::MIME_DATA_KEY].toString();
    QByteArray data = itemMap[ClipbirdQmlApplicationClipboard::DATA_KEY].toByteArray();
    clipboardData.append(qMakePair(mimeType, data));
  }
  m_syncingManager->synchronize(clipboardData);
}

ClipbirdQmlSyncingManager* ClipbirdQmlSyncingManager::create(QQmlEngine* engine, QJSEngine* scriptEngine) {
  static ClipbirdQmlSyncingManager* instance = new ClipbirdQmlSyncingManager(syncing::SyncingManagerFactory::getSyncingManager());
  Q_UNUSED(engine);
  Q_UNUSED(scriptEngine);
  return instance;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
