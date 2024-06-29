// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
/**
 * @brief Handle On Client State Changed (From server)
 */
void ClipBird::handleClientStateChanged(types::device::Device client, bool connected) {
  // if the host is not server then throw error
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // if not Connected the return
  if (!connected) return;

  // get the device certificate from the server
  auto cert = std::get<Server>(m_host).getClientCert(client);

  // get the store instance to store certificate
  auto &store = storage::Storage::instance();

  // store the client certificate
  store.setClientCert(client.name, cert.toPem());

  // add ca certificates
  this->m_sslConfig.addCaCertificate(cert);

  // set new config for Server
  std::get<Server>(m_host).setSslConfiguration(this->m_sslConfig);
}

/**
 * @brief Handle On Server Disconnect from Client (From client)
 *
 * @param isConnected is connected to the server
 */
void ClipBird::handleServerStatusChanged(bool status, types::device::Device host) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the client and disconnect the signals
  auto signal  = &clipboard::Clipboard::OnClipboardChange;
  auto slot    = &Client::syncItems;
  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  // if the client is connected then connect the signals
  if (status) {
    connect(&m_clipboard, signal, client, slot);
    auto cert = client->getConnectedServerCertificate();
    auto name = host.name;
    store.setServerCert(name, cert.toPem());
    this->m_sslConfig.addCaCertificate(cert);
    client->setSslConfiguration(this->m_sslConfig);
    return;
  }

  // Disconnect the Signal
  disconnect(&m_clipboard, signal, client, slot);

  // Get all server
  for (auto s : client->getServerList()) {
    if (host != s && store.hasServerCert(s.name)) {
      return client->connectToServerSecured(s);
    }
  }
}

/**
 * @brief Handle the Server Found (From client)
 */
void ClipBird::handleServerFound(types::device::Device server) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the connected server
  auto connectedServer = std::get<Client>(m_host).getConnectedServer();

  // if already connected then return
  if (connectedServer.has_value()) return;

  // get the client and store the server
  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  // if the server is not found then return
  if (store.hasServerCert(server.name)) {
    client->connectToServerSecured(server);
  }
}

/**
 * @brief Handle the sync request (From server)
 */
void ClipBird::handleSyncRequest(QVector<QPair<QString, QByteArray>> data) {
  // remove last
  if (this->m_history.size() + 1 > constants::getAppMaxHistorySize()) {
    this->m_history.pop_back();
  }

  // add to front
  this->m_history.push_front(data);

  // emit the signal
  emit OnHistoryChanged(m_history);
}

/**
 * @brief Handle the Auth Request (From Server)
 */
void ClipBird::handleAuthRequest(types::device::Device host) {
  // if the host is not server then throw error
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // get the certificate from the server
  auto cert = std::get<Server>(m_host).getUnauthedClientCert(host);

  // get the store instance
  auto &store = storage::Storage::instance();

  // is store not has the certificate
  if (!store.hasClientCert(host.name)) {
    return emit OnAuthRequest(host);
  }

  // get the certificate from the store
  auto c = store.getClientCert(host.name);

  // if the certificate is not same then emit the signal
  if (c != cert.toPem()) {
    return emit OnAuthRequest(host);
  } else {
    return this->authSuccess(host);
  }
}

/**
 * @brief Set the SSL Configuration object
 */
void ClipBird::setSslConfiguration(QSslConfiguration config) {
  // storage instance
  storage::Storage &store = storage::Storage::instance();

  // Ca Certificates
  QList<QSslCertificate> caCerts;

  // set all ca Client certs from store
  for (auto certByte : store.getAllClientCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // set all ca Server certs from store
  for (auto certByte : store.getAllServerCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // set ca certificates
  config.setCaCertificates(caCerts);

  // set the ssl configuration
  this->m_sslConfig = config;
}

/**
 * @brief Construct a new ClipBird object and manage
 * the clipboard, server and client
 *
 * @param board  clipboard that is managed
 * @param parent parent object
 */
ClipBird::ClipBird(QSslConfiguration config, QObject *parent) : QObject(parent), m_clipboard(this) {
  this->setSslConfiguration(config);
}

//---------------------- public slots -----------------------//

/**
 * @brief set the host as server and start listening
 * to accept the client
 */
void ClipBird::setCurrentHostAsServer() {
  // Emplace the server into the m_host variant variable
  auto *server = &m_host.emplace<Server>(this);

  // Set the QSslConfiguration
  server->setSslConfiguration(m_sslConfig);

  // Connect the onClientStateChanged signal to the signal
  connect(
    server, &Server::OnCLientStateChanged,
    this, &ClipBird::OnCLientStateChanged
  );

  connect(
    server, &Server::OnCLientStateChanged,
    this, &ClipBird::handleClientStateChanged
  );

  // connect OnAuthRequest to clipbird OnAuthRequest
  // Connect the onSyncRequest signal to the clipboard
  connect(
    server,  &Server::OnAuthRequest,
    this, &ClipBird::handleAuthRequest
  );

  // Connect the onSyncRequest signal to the clipboard
  connect(
    server, &Server::OnSyncRequest,
    this, &ClipBird::OnSyncRequest
  );

  connect(
    server, &Server::OnSyncRequest,
    this, &ClipBird::handleSyncRequest
  );

  connect(
    server, &Server::OnSyncRequest,
    &m_clipboard, &clipboard::Clipboard::set
  );

  // connect the OnClipboardChange signal to the server
  connect(
    &m_clipboard, &clipboard::Clipboard::OnClipboardChange,
    server, &Server::syncItems
  );

  // Connect the onClientListChanged signal to the signal
  connect(
    server, &Server::OnClientListChanged,
    this, &ClipBird::OnClientListChanged
  );

  // Connect the onServerStateChanged signal to the signal
  connect(
    server, &Server::OnServerStateChanged,
    this, &ClipBird::OnServerStateChanged
  );

  // get the storage instance
  auto &store = storage::Storage::instance();

  // set the host is client
  store.setHostIsServer(true);

  // Start the server to listen and accept the client
  server->startServer();

  // emit the signal for host change
  emit OnHostTypeChanged(types::enums::HostType::SERVER);
}

/**
 * @brief set the host as client
 */
void ClipBird::setCurrentHostAsClient() {
  // Emplace the client into the m_host variant variable
  auto *client         = &m_host.emplace<Client>(this);

  // Set the QSslConfiguration
  client->setSslConfiguration(m_sslConfig);

  // Connect the onServerListChanged signal to the signal
  connect(
    client, &Client::OnServerListChanged,
    this, &ClipBird::OnServerListChanged
  );

  // Connect the onServerFound signal to the signal
  connect(
    client, &Client::OnServerFound,
    this, &ClipBird::handleServerFound
  );

  connect(
    client, &Client::OnServerFound,
    this, &ClipBird::OnServerFound
  );

  // Connect the OnServerGone signal to the signal
  connect(
    client, &Client::OnServerGone,
    this, &ClipBird::OnServerGone
  );

  // Connect the onServerStateChanged signal to the signal
  connect(
    client, &Client::OnServerStatusChanged,
    this, &ClipBird::OnServerStatusChanged
  );

  connect(
    client, &Client::OnServerStatusChanged,
    this, &ClipBird::handleServerStatusChanged
  );

  // Connect the onSyncRequest signal to the clipboard
  connect(
    client, &Client::OnSyncRequest,
    this, &ClipBird::OnSyncRequest
  );

  connect(
    client, &Client::OnSyncRequest,
    this, &ClipBird::handleSyncRequest
  );

  connect(
    client, &Client::OnSyncRequest,
    &m_clipboard, &clipboard::Clipboard::set
  );

  // connect onConnectionError to the signal
  connect(
    client, &Client::OnConnectionError,
    this, &ClipBird::OnConnectionError
  );

  // get the storage instance
  auto &store = storage::Storage::instance();

  // set the host is client
  store.setHostIsServer(false);

  // Start the Discovery
  client->startBrowsing();

  // emit the signal for host change
  emit OnHostTypeChanged(types::enums::HostType::CLIENT);
}

/**
 * @brief Clear the Server Certificates
 */
void ClipBird::clearServerCertificates() {
  // storage instance
  storage::Storage &store = storage::Storage::instance();

  // Ca Certificates
  QList<QSslCertificate> caCerts;

  // set all ca Client certs from store
  for (auto certByte : store.getAllClientCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // clear all ca Server certs from store
  store.clearAllServerCert();

  // set the ca certificates
  this->m_sslConfig.setCaCertificates(caCerts);

  // if the host is server then set the ssl configuration
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).setSslConfiguration(this->m_sslConfig);
  }

  // if the host is client then set the ssl configuration
  if (std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).setSslConfiguration(this->m_sslConfig);
  }
}

/**
 * @brief Clear the Client Certificates
 */
void ClipBird::clearClientCertificates() {
 // storage instance
  storage::Storage &store = storage::Storage::instance();

  // Ca Certificates
  QList<QSslCertificate> caCerts;

  // set all ca Client certs from store
  for (auto certByte : store.getAllServerCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // set the ca certificates
  this->m_sslConfig.setCaCertificates(caCerts);

  // clear all ca Server certs from store
  store.clearAllClientCert();

  // if the host is server then set the ssl configuration
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).setSslConfiguration(this->m_sslConfig);
  }

  // if the host is client then set the ssl configuration
  if (std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).setSslConfiguration(this->m_sslConfig);
  }
}

//---------------------- Server functions -----------------------//

/**
 * @brief Get the Clients that are connected to the server
 *
 * @return QList<QSslSocket*> List of clients
 */
QList<types::device::Device> ClipBird::getConnectedClientsList() const {
  if (std::holds_alternative<Server>(m_host)) {
    return std::get<Server>(m_host).getConnectedClientsList();
  } else {
    throw std::runtime_error("Host is not server");
  }
}

/**
 * @brief Disconnect the client from the server and delete
 * the client
 * @param host ip address of the client
 * @param ip port number of the client
 */
void ClipBird::disconnectClient(const types::device::Device &client) {
  // if the host is not server then throw
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // find the client with the given host and ip
  const auto match = [&](auto i) -> auto{
    return (i.ip == client.ip) && (i.port == client.port);
  };

  // get the list of clients
  auto clients = getConnectedClientsList();

  // find the client
  auto it      = std::find_if(clients.begin(), clients.end(), match);

  // if the client is found then disconnect
  if (it != clients.end()) {
    std::get<Server>(m_host).disconnectClient(client);
  } else {
    throw std::runtime_error("Client not found");
  }
}

/**
 * @brief Disconnect the all the clients from the server
 */
void ClipBird::disconnectAllClients() {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).disconnectAllClients();
  } else {
    throw std::runtime_error("Host is not server");
  }
}

/**
 * @brief Get the server QHostAddress and port
 */
types::device::Device ClipBird::getServerInfo() const {
  if (std::holds_alternative<Server>(m_host)) {
    return std::get<Server>(m_host).getServerInfo();
  } else {
    throw std::runtime_error("Host is not server");
  }
}

/**
 * @brief Dispose Server
 */
void ClipBird::disposeServer() {
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // get the server
  auto *server = &std::get<Server>(m_host);

  // disconnect all clients
  server->disconnectAllClients();
}

/**
 * @brief The function that is called when the client is authenticated
 *
 * @param client the client that is currently processed
 */
void ClipBird::authSuccess(const types::device::Device &client) {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).authSuccess(client);
  } else {
    throw std::runtime_error("Host is not server");
  }
}

/**
 * @brief The function that is called when the client it not
 * authenticated
 *
 * @param client the client that is currently processed
 */
void ClipBird::authFailed(const types::device::Device &client) {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).authFailed(client);
  } else {
    throw std::runtime_error("Host is not server");
  }
}

//---------------------- Client functions -----------------------//

/**
 * @brief Get the Server List object
 *
 * @return QList<types::device::Device> List of servers
 */
QList<types::device::Device> ClipBird::getServerList() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getServerList();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Connect to the server with the given host and port
 * number
 *
 * @param host Host address
 * @param port Port number
 */
void ClipBird::connectToServer(const types::device::Device &host) {
  if (std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).connectToServer(host);
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief get the connected server address and port
 *
 * @return types::device::Device address and port
 */
std::optional<types::device::Device> ClipBird::getConnectedServer() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getConnectedServer();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Disconnect from the server
 */
void ClipBird::disconnectFromServer(const types::device::Device &host) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // find the server with the given host and ip
  const auto match = [&](auto i) -> auto{
    return (i->ip == host.ip) && (i->port == host.port);
  };

  // get the connected server
  auto server = getConnectedServer();

  // if the server is found then disconnect
  if (match(server)) {
    std::get<Client>(m_host).disconnectFromServer();
  } else {
    throw std::runtime_error("Server not found");
  }
}

/**
 * @brief Dispose Client
 */
void ClipBird::disposeClient() {
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the client
  auto *client = &std::get<Client>(m_host);

  // disconnect from the server
  client->disconnectFromServer();
}

/**
 * @brief Sync the clipboard data with the Group
 */
void ClipBird::syncClipboard(const QVector<QPair<QString, QByteArray>> &data) {
  if(std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).syncItems(data);
  }

  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).syncItems(data);
  }
}

/**
 * @brief Get the Clipboard data
 *
 * @return clipboard::Clipboard& clipboard
 */
QVector<QPair<QString, QByteArray>> ClipBird::getClipboard() const {
  return m_clipboard.get();
}

/**
 * @brief Set the Clipboard data
 *
 * @param data clipboard data
 */
void ClipBird::setClipboard(const QVector<QPair<QString, QByteArray>> &data) {
  m_clipboard.set(data);
}

/**
 * @brief IS the Host is Lastly Server
 */
bool ClipBird::isLastlyHostIsServer() const {
  return storage::Storage::instance().getHostIsServer();
}

/**
 * @brief delete the history at the given index
 */
void ClipBird::deleteHistoryAt(int index) {
  // if the index is out of range then throw error
  if (index < 0 || index >= m_history.size()) {
    throw std::runtime_error("Index out of range");
  }

  // remove the history at the given index
  m_history.remove(index);

  // emit the signal
  emit OnHistoryChanged(m_history);
}

/**
 * @brief Get the History of the clipboard
 */
QVector<QVector<QPair<QString, QByteArray>>> ClipBird::getHistory() const {
  return m_history;
}

/**
 * @brief Get the Host Type
 */
types::enums::HostType ClipBird::getHostType() const {
  if (std::holds_alternative<Client>(m_host)) {
    return types::enums::HostType::CLIENT;
  }

  if (std::holds_alternative<Server>(m_host)) {
    return types::enums::HostType::SERVER;
  }

  return types::enums::HostType::NONE;
}
}  // namespace srilakshmikanthanp::clipbirdesk::controller
