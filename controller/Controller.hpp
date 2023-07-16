#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QObject>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QHostInfo>

// C++ headers
#include <functional>
#include <variant>

// project headers
#include "clipboard/Clipboard.hpp"
#include "network/syncing/SyncingClient.hpp"
#include "network/syncing/SyncingServer.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class Controller : public QObject {
 private:  // typedefs for this class
  using Authenticator = std::function<bool(QPair<QHostAddress, quint16>)>;
  using SyncingServer = network::syncing::SyncingServer;
  using SyncingClient = network::syncing::SyncingClient;

  //----------------------- client Signals -------------------------//

 signals:    // signals for this class
  /// @brief On Server List Changed (From Client)
  void OnServerListChanged(QList<QPair<QHostAddress, quint16>> servers);

 signals:    // signals for this class
  /// @brief On Server Found  (From Client)
  void OnServerFound(QPair<QHostAddress, quint16> server);

 signals:    // signals for this class
  /// @brief On Server state changed (From Client)
  void OnServerStatusChanged(bool isConnected);

  //----------------------- general Signals -----------------------//

 signals:    // signals for this class
  /// @brief On Error Occurred (General)
  void OnErrorOccurred(QString error);

  //----------------------- server Signals ------------------------//

 signals:    // signals
  /// @brief On client state changed (From Server)
  void OnCLientStateChanged(QPair<QHostAddress, quint16> client, bool connected);

 signals:    // signals for this class
  /// @brief On Server state changed (From Server)
  void OnServerStateChanged(bool isStarted);

 signals:    // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnClientListChanged(QList<QPair<QHostAddress, quint16>> clients);

 private:    // just for Qt
  Q_OBJECT

 private:    // Member variable
  std::variant<SyncingServer, SyncingClient> m_host;
  QSslConfiguration m_sslConfig;
  clipboard::Clipboard m_clipboard;
  Authenticator m_authenticator = nullptr;

 public:   // slots
  /**
   * @brief set the host as server and start listening
   * to accept the client
   */
  void setCurrentHostAsServer() {
    // Emplace the server into the m_host variant variable
    auto *server = &m_host.emplace<SyncingServer>(this);

    // if the authenticator is not set then throw error
    if (m_authenticator != nullptr) {
      server->setAuthenticator(m_authenticator);
    }

    // Set the QSslConfiguration
    server->setSSLConfiguration(m_sslConfig);

    // Connect the onClientStateChanged signal to the signal
    const auto signal_s = &SyncingServer::OnCLientStateChanged;
    const auto slot_s = &Controller::OnCLientStateChanged;
    connect(server, signal_s, this, slot_s);

    // Connect the onSyncRequest signal to the clipboard
    const auto signal_c = &SyncingServer::OnSyncRequest;
    const auto slot_c = &clipboard::Clipboard::set;
    connect(server, signal_c, &m_clipboard, slot_c);

    // connect the OnClipboardChange signal to the server
    const auto signal_b = &clipboard::Clipboard::OnClipboardChange;
    const auto slot_b = &SyncingServer::syncItems;
    connect(&m_clipboard, signal_b, server, slot_b);

    // Connect the onErrorOccurred signal to the signal
    const auto signal_e = &SyncingServer::OnErrorOccurred;
    const auto slot_e = &Controller::OnErrorOccurred;
    connect(server, signal_e, this, slot_e);

    // Connect the onClientListChanged signal to the signal
    const auto signal_l = &SyncingServer::OnClientListChanged;
    const auto slot_l = &Controller::OnClientListChanged;
    connect(server, signal_l, this, slot_l);

    // Connect the onServerStateChanged signal to the signal
    const auto signal_t = &SyncingServer::OnServerStateChanged;
    const auto slot_t = &Controller::OnServerStateChanged;
    connect(server, signal_t, this, slot_t);

    // Start the server to listen and accept the client
    server->start();
  }

  /**
   * @brief set the host as client
   */
  void setCurrentHostAsClient() {
    // Emplace the client into the m_host variant variable
    auto *client = &m_host.emplace<SyncingClient>(this);

    // Set the SSL Configuration
    client->setSSLConfiguration(m_sslConfig);

    // Connect the onServerListChanged signal to the signal
    const auto signal_s = &SyncingClient::OnServerListChanged;
    const auto slot_s = &Controller::OnServerListChanged;
    connect(client, signal_s, this, slot_s);

    // Connect the onServerFound signal to the signal
    const auto signal_f = &SyncingClient::OnServerFound;
    const auto slot_f = &Controller::OnServerFound;
    connect(client, signal_f, this, slot_f);

    // Connect the onServerStateChanged signal to the signal
    const auto signal_c = &SyncingClient::OnServerStatusChanged;
    const auto slot_c = &Controller::OnServerStatusChanged;
    connect(client, signal_c, this, slot_c);

    // Connect the onErrorOccurred signal to the signal
    const auto signal_e = &SyncingClient::OnErrorOccurred;
    const auto slot_e = &Controller::OnErrorOccurred;
    connect(client, signal_e, this, slot_e);

    // Connect the onSyncRequest signal to the clipboard
    const auto signal_r = &SyncingClient::OnSyncRequest;
    const auto slot_r = &clipboard::Clipboard::set;
    connect(client, signal_r, &m_clipboard, slot_r);

    // Connect the onClipboardChange signal to the client
    const auto signal_b = &clipboard::Clipboard::OnClipboardChange;
    const auto slot_b = &SyncingClient::syncItems;
    connect(&m_clipboard, signal_b, client, slot_b);

    // Start the Discovery
    client->startDiscovery();
  }

 public:   // Member functions
  /**
   * @brief Construct a new Controller object and manage
   * the clipboard, server and client
   *
   * @param board  clipboard that is managed
   * @param parent parent object
   */
  Controller(QClipboard *board, QObject* parent = nullptr)
    : QObject(parent), m_clipboard(board, this) {}

  /**
   * @brief Destroy the Controller object
   */
  virtual ~Controller() = default;

  /**
   * @brief set the authenticator
   *
   * @param authenticator authenticator function
   */
  void setAuthenticator(Authenticator authenticator) {
    m_authenticator = authenticator;
  }

  /**
   * @brief get the authenticator
   *
   * @return Authenticator authenticator function
   */
  Authenticator getAuthenticator() const {
    return m_authenticator;
  }

  /**
   * @brief set the ssl configuration
   *
   * @param sslConfig ssl configuration
   */
  void setSSLConfiguration(QSslConfiguration sslConfig) {
    m_sslConfig = sslConfig;
  }

  /**
   * @brief get the ssl configuration
   *
   * @return QSslConfiguration ssl configuration
   */
  QSslConfiguration getSSLConfiguration() const {
    return m_sslConfig;
  }

  //---------------------- Server functions -----------------------//

  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  const QList<QPair<QHostAddress, quint16>> getConnectedClientsList() const {
    if (std::holds_alternative<SyncingServer>(m_host)) {
      return std::get<SyncingServer>(m_host).getConnectedClientsList();
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
  void disconnectClient(QPair<QHostAddress, quint16> client) {
    // if the host is not server then throw
    if (!std::holds_alternative<SyncingServer>(m_host)) {
      throw std::runtime_error("Host is not server");
    }

    // find the client with the given host and ip
    const auto match = [&](auto i) -> auto {
      return i.first == client.first && i.second == client.second;
    };

    // get the list of clients
    auto clients = getConnectedClientsList();

    // find the client
    auto it = std::find_if(clients.begin(), clients.end(), match);

    // if the client is found then disconnect
    if (it != clients.end()) {
      std::get<SyncingServer>(m_host).disconnectClient(client);
    } else {
      throw std::runtime_error("Client not found");
    }
  }

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients() {
    if (std::holds_alternative<SyncingServer>(m_host)) {
      std::get<SyncingServer>(m_host).disconnectAllClients();
    } else {
      throw std::runtime_error("Host is not server");
    }
  }

  /**
   * @brief Get the server QHostAddress and port
   */
  QPair<QHostAddress, quint16> getServer() {
    if (std::holds_alternative<SyncingServer>(m_host)) {
      return std::get<SyncingServer>(m_host).getServerInfo();
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
  QList<QPair<QHostAddress, quint16>> getServerList() {
    if (std::holds_alternative<SyncingClient>(m_host)) {
      return std::get<SyncingClient>(m_host).getServerList();
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
  void connectToServer(QPair<QHostAddress, quint16> host) {
    if (std::holds_alternative<SyncingClient>(m_host)) {
      std::get<SyncingClient>(m_host).connectToServer(host);
    } else {
      throw std::runtime_error("Host is not client");
    }
  }

  /**
   * @brief get the connected server address and port
   *
   * @return QPair<QHostAddress, quint16> address and port
   */
  QPair<QHostAddress, quint16> getConnectedServer() {
    if (std::holds_alternative<SyncingClient>(m_host)) {
      return std::get<SyncingClient>(m_host).getConnectedServer();
    } else {
      throw std::runtime_error("Host is not client");
    }
  }

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer(QPair<QHostAddress, quint16> host) {
    // if the host is not client then throw error
    if (!std::holds_alternative<SyncingClient>(m_host)) {
      throw std::runtime_error("Host is not client");
    }

    // find the server with the given host and ip
    const auto match = [&](auto i) -> auto {
      return i.first == host.first && i.second == host.second;
    };

    // Get the list of servers
    auto servers = this->getServerList();

    // find the server
    auto it = std::find_if(servers.begin(), servers.end(), match);

    // if the server is not found then throw error
    if (it == servers.end()) {
      throw std::runtime_error("Server not found");
    }

    // disconnect from the server
    std::get<SyncingClient>(m_host).disconnectFromServer();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
