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
  const auto slot_l = &ClipBird::OnClipboardSent;

  // if the client is connected then connect the signals
  if (isSuccess) {
    connect(&m_clipboard, signal, client, slot_n);
    connect(&m_clipboard, signal, this, slot_l);
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
  const auto slot_l = &ClipBird::OnClipboardSent;

  // if the client is connected then connect the signals
  if (!status) {
    disconnect(&m_clipboard, signal, client, slot_n);
    disconnect(&m_clipboard, signal, this, slot_l);
  }
}

/**
 * @brief Construct a new ClipBird object and manage
 * the clipboard, server and client
 *
 * @param board  clipboard that is managed
 * @param parent parent object
 */
ClipBird::ClipBird(QClipboard *board, QSslConfiguration config, QObject *parent)
    : QObject(parent), m_clipboard(board, this), m_sslConfig(config) {}

//---------------------- public slots -----------------------//

/**
 * @brief set the host as server and start listening
 * to accept the client
 */
void ClipBird::setCurrentHostAsServer() {
  // Emplace the server into the m_host variant variable
  auto *server = &m_host.emplace<Server>(this);

  // Set the QSslConfiguration
  server->setSSLConfiguration(m_sslConfig);

  // Connect the onClientStateChanged signal to the signal
  const auto signal_s = &Server::OnCLientStateChanged;
  const auto slot_s   = &ClipBird::OnCLientStateChanged;
  connect(server, signal_s, this, slot_s);

  // Connect the onNewHostConnected signal to the signal
  const auto signal_n = &Server::OnNewHostConnected;
  const auto slot_n   = &ClipBird::OnNewHostConnected;
  connect(server, signal_n, this, slot_n);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_c = &Server::OnSyncRequest;
  const auto slot_c   = &clipboard::Clipboard::set;
  const auto slot_g   = &ClipBird::OnClipboardRecv;
  connect(server, signal_c, &m_clipboard, slot_c);
  connect(server, signal_c, this, slot_g);

  // connect the OnClipboardChange signal to the server
  const auto signal_b = &clipboard::Clipboard::OnClipboardChange;
  const auto slot_b   = &Server::syncItems;
  const auto slot_x   = &ClipBird::OnClipboardSent;
  connect(&m_clipboard, signal_b, server, slot_b);
  connect(&m_clipboard, signal_b, this, slot_x);

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
  auto *client         = &m_host.emplace<Client>(this);

  // Connect the onServerListChanged signal to the signal
  const auto signal_sl = &Client::OnServerListChanged;
  const auto slot_sl   = &ClipBird::OnServerListChanged;
  connect(client, signal_sl, this, slot_sl);

  // Connect the onServerFound signal to the signal
  const auto signal_fn = &Client::OnServerFound;
  const auto slot_fn   = &ClipBird::OnServerFound;
  connect(client, signal_fn, this, slot_fn);

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

  // Connect the onErrorOccurred signal to the signal
  const auto signal_er = &Client::OnErrorOccurred;
  const auto slot_er   = &ClipBird::OnErrorOccurred;
  connect(client, signal_er, this, slot_er);

  // Connect the onSyncRequest signal to the clipboard
  const auto signal_rq = &Client::OnSyncRequest;
  const auto slot_rq   = &clipboard::Clipboard::set;
  const auto slot_g    = &ClipBird::OnClipboardRecv;
  connect(client, signal_rq, &m_clipboard, slot_rq);
  connect(client, signal_rq, this, slot_g);

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
void ClipBird::disconnectClient(const QPair<QHostAddress, quint16> &client) {
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
 * @brief The function that is called when the client is authenticated
 *
 * @param client the client that is currently processed
 */
void ClipBird::authSuccess(const QPair<QHostAddress, quint16> &client) {
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
void ClipBird::authFailed(const QPair<QHostAddress, quint16> &client) {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).authFailed(client);
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
void ClipBird::connectToServer(const QPair<QHostAddress, quint16> &host) {
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
 * @brief get the connected server address and port or empty
 *
 * @return QPair<QHostAddress, quint16> address and port
 */
QPair<QHostAddress, quint16> ClipBird::getConnectedServerOrEmpty() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getConnectedServerOrEmpty();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Disconnect from the server
 */
void ClipBird::disconnectFromServer(const QPair<QHostAddress, quint16> &host) {
  // if the host is not client then throw error
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  // find the server with the given host and ip
  const auto match = [&](auto i) -> auto{
    return i.first == host.first && i.second == host.second;
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
 * @brief Get the Authed Server object
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> ClipBird::getAuthedServer() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getAuthedServer();
  } else {
    throw std::runtime_error("Host is not client");
  }
}

/**
 * @brief Get the Authed Server object Or Empty
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> ClipBird::getAuthedServerOrEmpty() const {
  if (std::holds_alternative<Client>(m_host)) {
    return std::get<Client>(m_host).getAuthedServerOrEmpty();
  } else {
    throw std::runtime_error("Host is not client");
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::controller
