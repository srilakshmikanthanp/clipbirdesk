// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
/**
 * @brief Handle On Server Status Changed (From client)
 *
 * @param isConnected is connected to the server
 */
void ClipBird::handleServerStatusChanged(bool isConnected) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // get the client
  auto *client = &std::get<Client>(m_host);

  // if the client is connected then connect the signals
  if (isConnected) {
    // Connect the onClipboardChange signal to the client
    const auto signal = &clipboard::Clipboard::OnClipboardChange;
    const auto slot   = &Client::syncItems;
    connect(&m_clipboard, signal, client, slot);
  } else {
    // Disconnect the onClipboardChange signal to the client
    const auto signal = &clipboard::Clipboard::OnClipboardChange;
    const auto slot   = &Client::syncItems;
    disconnect(&m_clipboard, signal, client, slot);
  }
}

/**
 * @brief Construct a new ClipBird object and manage
 * the clipboard, server and client
 *
 * @param board  clipboard that is managed
 * @param parent parent object
 */
ClipBird::ClipBird(QClipboard *board, QObject *parent)
    : QObject(parent), m_clipboard(board, this) {}

/**
 * @brief set the authenticator
 *
 * @param authenticator authenticator function
 */
void ClipBird::setAuthenticator(Authenticator authenticator) {
  m_authenticator = authenticator;
}

/**
 * @brief get the authenticator
 *
 * @return Authenticator authenticator function
 */
ClipBird::Authenticator ClipBird::getAuthenticator() const {
  return m_authenticator;
}

/**
 * @brief set the ssl configuration
 *
 * @param sslConfig ssl configuration
 */
void ClipBird::setSSLConfiguration(QSslConfiguration sslConfig) {
  m_sslConfig = sslConfig;
}

/**
 * @brief get the ssl configuration
 *
 * @return QSslConfiguration ssl configuration
 */
QSslConfiguration ClipBird::getSSLConfiguration() const {
  return m_sslConfig;
}

//---------------------- public slots -----------------------//

/**
 * @brief set the host as server and start listening
 * to accept the client
 */
void ClipBird::setCurrentHostAsServer() {
  // Emplace the server into the m_host variant variable
  auto *server = &m_host.emplace<Server>(this);

  // if the authenticator is not set then throw error
  if (m_authenticator != nullptr) {
    server->setAuthenticator(m_authenticator);
  }

  // Set the QSslConfiguration
  server->setSSLConfiguration(m_sslConfig);

  // Connect the onClientStateChanged signal to the signal
  const auto signal_s = &Server::OnCLientStateChanged;
  const auto slot_s   = &ClipBird::OnCLientStateChanged;
  connect(server, signal_s, this, slot_s);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_c = &Server::OnSyncRequest;
  const auto slot_c   = &clipboard::Clipboard::set;
  connect(server, signal_c, &m_clipboard, slot_c);

  // connect the OnClipboardChange signal to the server
  const auto signal_b = &clipboard::Clipboard::OnClipboardChange;
  const auto slot_b   = &Server::syncItems;
  connect(&m_clipboard, signal_b, server, slot_b);

  // Connect the onErrorOccurred signal to the signal
  const auto signal_e = &Server::OnErrorOccurred;
  const auto slot_e   = &ClipBird::OnErrorOccurred;
  connect(server, signal_e, this, slot_e);

  // Connect the onClientListChanged signal to the signal
  const auto signal_l = &Server::OnClientListChanged;
  const auto slot_l   = &ClipBird::OnClientListChanged;
  connect(server, signal_l, this, slot_l);

  // Connect the onServerStateChanged signal to the signal
  const auto signal_t = &Server::OnServerStateChanged;
  const auto slot_t   = &ClipBird::OnServerStateChanged;
  connect(server, signal_t, this, slot_t);

  // Start the server to listen and accept the client
  server->startServer();
}

/**
 * @brief set the host as client
 */
void ClipBird::setCurrentHostAsClient() {
  // Emplace the client into the m_host variant variable
  auto *client = &m_host.emplace<Client>(this);

  // Set the SSL Configuration
  client->setSSLConfiguration(m_sslConfig);

  // Connect the onServerListChanged signal to the signal
  const auto signal_s = &Client::OnServerListChanged;
  const auto slot_s   = &ClipBird::OnServerListChanged;
  connect(client, signal_s, this, slot_s);

  // Connect the onServerFound signal to the signal
  const auto signal_f = &Client::OnServerFound;
  const auto slot_f   = &ClipBird::OnServerFound;
  connect(client, signal_f, this, slot_f);

  // Connect the onServerStateChanged signal to the signal
  const auto signal_c = &Client::OnServerStatusChanged;
  const auto slot_c   = &ClipBird::handleServerStatusChanged;
  const auto slot_d   = &ClipBird::handleServerStatusChanged;
  connect(client, signal_c, this, slot_c);
  connect(client, signal_c, this, slot_d);

  // Connect the onErrorOccurred signal to the signal
  const auto signal_e = &Client::OnErrorOccurred;
  const auto slot_e   = &ClipBird::OnErrorOccurred;
  connect(client, signal_e, this, slot_e);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_r = &Client::OnSyncRequest;
  const auto slot_r   = &clipboard::Clipboard::set;
  connect(client, signal_r, &m_clipboard, slot_r);

  // Start the Discovery
  client->startDiscovery();
}

//---------------------- Server functions -----------------------//

/**
 * @brief Get the Clients that are connected to the server
 *
 * @return QList<QSslSocket*> List of clients
 */
QList<QPair<QHostAddress, quint16>> ClipBird::getConnectedClientsList() const {
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
void ClipBird::disconnectClient(QPair<QHostAddress, quint16> client) {
  // if the host is not server then throw
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  // find the client with the given host and ip
  const auto match = [&](auto i) -> auto{
    return i.first == client.first && i.second == client.second;
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
QPair<QHostAddress, quint16> ClipBird::getServerInfo() const {
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
 * @return QList<QPair<QHostAddress, quint16>> List of servers
 */
QList<QPair<QHostAddress, quint16>> ClipBird::getServerList() const {
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
void ClipBird::connectToServer(QPair<QHostAddress, quint16> host) {
  if (std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).connectToServer(host);
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief get the connected server address and port
 *
 * @return QPair<QHostAddress, quint16> address and port
 */
QPair<QHostAddress, quint16> ClipBird::getConnectedServer() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getConnectedServer();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Disconnect from the server
 */
void ClipBird::disconnectFromServer(QPair<QHostAddress, quint16> host) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // find the server with the given host and ip
  const auto match = [&](auto i) -> auto{
    return i.first == host.first && i.second == host.second;
  };

  // Get the list of servers
  auto servers = this->getServerList();

  // find the server
  auto it      = std::find_if(servers.begin(), servers.end(), match);

  // if the server is not found then throw error
  if (it == servers.end()) {
    throw std::runtime_error("Server not found");
  }

  // disconnect from the server
  std::get<Client>(m_host).disconnectFromServer();
}
}  // namespace srilakshmikanthanp::clipbirdesk::controller
