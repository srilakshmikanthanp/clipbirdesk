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
void ClipBird::handleServerStatusChanged(bool status) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the client and disconnect the signals
  auto signal  = &clipboard::Clipboard::OnClipboardChange;
  auto *client = &std::get<Client>(m_host);
  auto slot_n  = &Client::syncItems;
  auto &store  = storage::Storage::instance();

  // if the client is connected then connect the signals
  if (!status) {
    disconnect(&m_clipboard, signal, client, slot_n);
  } else {
    connect(&m_clipboard, signal, client, slot_n);
    auto cert = client->getConnectedServerCertificate();
    auto name = client->getConnectedServer().name;
    store.setServerCert(name, cert.toPem());
    this->m_sslConfig.addCaCertificate(cert);
    client->setSslConfiguration(this->m_sslConfig);
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

  // if already connected then return
  if (std::get<Client>(m_host).isConnected()) return;

  // get the client and store the server
  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  // if the server is not found then return
  if (store.hasServerCert(server.name)) {
    client->connectToServerSecured(server);
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
  const auto slot_hcs  = &ClipBird::handleClientStateChanged;
  const auto signal_cs = &Server::OnCLientStateChanged;
  const auto slot_cs   = &ClipBird::OnCLientStateChanged;
  connect(server, signal_cs, this, slot_cs);
  connect(server, signal_cs, this, slot_hcs);

  // connect OnAuthRequest to clipbird OnAuthRequest
  // Connect the onSyncRequest signal to the clipboard
  const auto signal_sa = &Server::OnAuthRequest;
  const auto slot_sa   = &ClipBird::OnAuthRequest;
  connect(server, signal_sa, this, slot_sa);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_sr = &Server::OnSyncRequest;
  const auto slot_sr   = &clipboard::Clipboard::set;
  connect(server, signal_sr, &m_clipboard, slot_sr);

  // connect the OnClipboardChange signal to the server
  const auto signal_cc = &clipboard::Clipboard::OnClipboardChange;
  const auto slot_si   = &Server::syncItems;
  connect(&m_clipboard, signal_cc, server, slot_si);

  // Connect the onClientListChanged signal to the signal
  const auto signal_lc = &Server::OnClientListChanged;
  const auto slot_lc   = &ClipBird::OnClientListChanged;
  connect(server, signal_lc, this, slot_lc);

  // Connect the onServerStateChanged signal to the signal
  const auto signal_ss = &Server::OnServerStateChanged;
  const auto slot_ss   = &ClipBird::OnServerStateChanged;
  connect(server, signal_ss, this, slot_ss);

  // get the storage instance
  auto &store = storage::Storage::instance();

  // set the host is client
  store.setHostIsServer(true);

  // Start the server to listen and accept the client
  server->startServer();
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
  const auto signal_sl = &Client::OnServerListChanged;
  const auto slot_sl   = &ClipBird::OnServerListChanged;
  connect(client, signal_sl, this, slot_sl);

  // Connect the onServerFound signal to the signal
  const auto signal_fn = &Client::OnServerFound;
  const auto slot_hfn  = &ClipBird::handleServerFound;
  const auto slot_fn   = &ClipBird::OnServerFound;
  connect(client, signal_fn, this, slot_hfn);
  connect(client, signal_fn, this, slot_fn);

  // Connect the OnServerGone signal to the signal
  const auto signal_sg = &Client::OnServerGone;
  const auto slot_sg   = &ClipBird::OnServerGone;
  connect(client, signal_sg, this, slot_sg);

  // Connect the onServerStateChanged signal to the signal
  const auto slot_hc   = &ClipBird::handleServerStatusChanged;
  const auto signal_sc = &Client::OnServerStatusChanged;
  const auto slot_sc   = &ClipBird::OnServerStatusChanged;
  connect(client, signal_sc, this, slot_sc);
  connect(client, signal_sc, this, slot_hc);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_rq = &Client::OnSyncRequest;
  const auto slot_rq   = &clipboard::Clipboard::set;
  connect(client, signal_rq, &m_clipboard, slot_rq);

  // connect onConnectionError to the signal
  const auto signal_ce = &Client::OnConnectionError;
  const auto slot_ce   = &ClipBird::OnConnectionError;
  connect(client, signal_ce, this, slot_ce);

  // get the storage instance
  auto &store = storage::Storage::instance();

  // set the host is client
  store.setHostIsServer(false);

  // Start the Discovery
  client->startBrowsing();
}

/**
 * @brief Clear the Server Certificates
 */
void ClipBird::clearServerCertificates() {
  // storage instance
  storage::Storage &store = storage::Storage::instance();

  // Ca Certificates
  QList<QSslCertificate> caCerts;

  // clear all ca Server certs from m_sslconfig
  this->m_sslConfig.setCaCertificates(caCerts);

  // set all ca Client certs from store
  for (auto certByte : store.getAllClientCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // set the ca certificates
  this->m_sslConfig.setCaCertificates(caCerts);

  // clear all ca Server certs from store
  store.clearAllServerCert();

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

  // clear all ca Server certs from m_sslconfig
  this->m_sslConfig.setCaCertificates(caCerts);

  // set all ca Client certs from store
  for (auto certByte : store.getAllServerCert()) {
    caCerts.append(QSslCertificate(certByte, QSsl::Pem));
  }

  // set the ca certificates
  this->m_sslConfig.setCaCertificates(caCerts);

  // clear all ca Server certs from store
  store.clearAllServerCert();

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
 * @brief Is Client Connected
 */
bool ClipBird::isConnectedToServer() {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).isConnected();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief get the connected server address and port
 *
 * @return types::device::Device address and port
 */
types::device::Device ClipBird::getConnectedServer() const {
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
    return (i.ip == host.ip) && (i.port == host.port);
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
 * @brief IS the Host is Lastly Server
 */
bool ClipBird::isLastlyHostIsServer() const {
  return storage::Storage::instance().getHostIsServer();
}
}  // namespace srilakshmikanthanp::clipbirdesk::controller
