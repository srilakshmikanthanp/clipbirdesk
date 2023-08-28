// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
/**
 * @brief Handle On Server Authenticated the Client (From client)
 *
 * @param isConnected is connected to the server
 */
void ClipBird::handleServerAuthentication(bool isSuccess) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the client and disconnect the signals
  const auto signal = &clipboard::Clipboard::OnClipboardChange;
  auto *client      = &std::get<Client>(m_host);
  const auto slot_n = &Client::syncItems;
  auto &store       = storage::Storage::instance();
  auto cert         = client->getAuthedServer().cert;
  auto name         = client->getAuthedServer().name;

  // if the client is connected then connect the signals
  if (isSuccess) {
    connect(&m_clipboard, signal, client, slot_n);
    store.setServerCert(name, cert.toPem());
  }
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
  const auto signal = &clipboard::Clipboard::OnClipboardChange;
  auto *client      = &std::get<Client>(m_host);
  const auto slot_n = &Client::syncItems;

  // if the client is connected then connect the signals
  if (!status) {
    disconnect(&m_clipboard, signal, client, slot_n);
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

  // get the client and store the server
  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  // if the server is not found then return
  if (!store.hasServerCert(server.name)) {
    return;
  }

  // get the cert from the storage
  auto cstr = store.getServerCert(server.name);
  auto cert = QSslCertificate::fromData(cstr).first();

  // if cert matches and not connected to server
  if (cert == server.cert && client->isConnected()) {
    client->connectToServer(server);
  }
}

/**
 * @brief Handle the Auth Request (From Server)
 */
void ClipBird::handleAuthRequest(types::device::Device client) {
  // if the host is not server then throw error
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // get the server and store
  auto *server = &std::get<Server>(m_host);
  auto &store  = storage::Storage::instance();

  // if client is not fount in store
  if (!store.hasClientCert(client.name)) {
    return emit this->OnAuthRequested(client);
  }

  // get the certificate from store
  auto cstr = store.getClientCert(client.name);
  auto cert = QSslCertificate::fromData(cstr).first();

  // if certificate matches
  if (cert == client.cert) {
    return server->authSuccess(client);
  }

  // again ask for user to Authenticate
  return emit this->OnAuthRequested(client);
}

/**
 * @brief Construct a new ClipBird object and manage
 * the clipboard, server and client
 *
 * @param board  clipboard that is managed
 * @param parent parent object
 */
ClipBird::ClipBird(QSslConfiguration config, QObject *parent)
    : QObject(parent), m_clipboard(this), m_sslConfig(config) {}

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
  const auto signal_cs = &Server::OnCLientStateChanged;
  const auto slot_cs   = &ClipBird::OnCLientStateChanged;
  connect(server, signal_cs, this, slot_cs);

  // Connect the onNewHostConnected signal to the signal
  const auto signal_nh = &Server::OnAuthRequested;
  const auto slot_nh   = &ClipBird::handleAuthRequest;
  connect(server, signal_nh, this, slot_nh);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_sr = &Server::OnSyncRequest;
  const auto slot_bs   = &clipboard::Clipboard::set;
  connect(server, signal_sr, &m_clipboard, slot_bs);

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

  // Connect the onServerAuthentication signal to the signal
  const auto slot_ha   = &ClipBird::handleServerAuthentication;
  const auto signal_au = &Client::OnServerAuthentication;
  const auto slot_au   = &ClipBird::OnServerAuthentication;
  connect(client, signal_au, this, slot_au);
  connect(client, signal_au, this, slot_ha);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_rq = &Client::OnSyncRequest;
  const auto slot_rq   = &clipboard::Clipboard::set;
  connect(client, signal_rq, &m_clipboard, slot_rq);

  // get the storage instance
  auto &store = storage::Storage::instance();

  // set the host is client
  store.setHostIsServer(false);

  // Start the Discovery
  client->startBrowsing();
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
 * @brief Is Authed Server
 */
bool ClipBird::isClientAuthed() {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).isAuthenticated();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Get the Authed Server object
 * @return types::device::Device
 */
types::device::Device ClipBird::getAuthedServer() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getAuthedServer();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief IS the Host is Lastly Server
 */
bool ClipBird::isLastlyHostIsServer() const {
  auto &store = storage::Storage::instance();
  return store.getHostIsServer();
}
}  // namespace srilakshmikanthanp::clipbirdesk::controller
