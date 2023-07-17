#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QHostInfo>
#include <QObject>
#include <QSslConfiguration>
#include <QSslSocket>

// C++ headers
#include <functional>
#include <variant>

// project headers
#include "clipboard/clipboard.hpp"
#include "network/syncing/client/syncingclient.hpp"
#include "network/syncing/server/syncingserver.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class Controller : public QObject {
 private:  // typedefs for this class

  using Authenticator = std::function<bool(QPair<QHostAddress, quint16>)>;
  using SyncingServer = network::syncing::SyncingServer;
  using SyncingClient = network::syncing::SyncingClient;

  //----------------------- client Signals -------------------------//

 signals:  // signals for this class
  /// @brief On Server List Changed (From Client)
  void OnServerListChanged(QList<QPair<QHostAddress, quint16>> servers);

 signals:  // signals for this class
  /// @brief On Server Found  (From Client)
  void OnServerFound(QPair<QHostAddress, quint16> server);

 signals:  // signals for this class
  /// @brief On Server state changed (From Client)
  void OnServerStatusChanged(bool isConnected);

  //----------------------- general Signals -----------------------//

 signals:  // signals for this class
  /// @brief On Error Occurred (General)
  void OnErrorOccurred(QString error);

  //----------------------- server Signals ------------------------//

 signals:  // signals
  /// @brief On client state changed (From Server)
  void OnCLientStateChanged(QPair<QHostAddress, quint16> client, bool connected);

 signals:  // signals for this class
  /// @brief On Server state changed (From Server)
  void OnServerStateChanged(bool isStarted);

 signals:  // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnClientListChanged(QList<QPair<QHostAddress, quint16>> clients);

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  std::variant<SyncingServer, SyncingClient> m_host;
  QSslConfiguration m_sslConfig;
  clipboard::Clipboard m_clipboard;
  Authenticator m_authenticator = nullptr;

 public:  // slots

  /**
   * @brief set the host as server and start listening
   * to accept the client
   */
  void setCurrentHostAsServer();

  /**
   * @brief set the host as client
   */
  void setCurrentHostAsClient();

 public:  // Member functions

  /**
   * @brief Construct a new Controller object and manage
   * the clipboard, server and client
   *
   * @param board  clipboard that is managed
   * @param parent parent object
   */
  Controller(QClipboard* board, QObject* parent = nullptr);

  /**
   * @brief Destroy the Controller object
   */
  virtual ~Controller() = default;

  /**
   * @brief set the authenticator
   *
   * @param authenticator authenticator function
   */
  void setAuthenticator(Authenticator authenticator);

  /**
   * @brief get the authenticator
   *
   * @return Authenticator authenticator function
   */
  Authenticator getAuthenticator() const;

  /**
   * @brief set the ssl configuration
   *
   * @param sslConfig ssl configuration
   */
  void setSSLConfiguration(QSslConfiguration sslConfig);

  /**
   * @brief get the ssl configuration
   *
   * @return QSslConfiguration ssl configuration
   */
  QSslConfiguration getSSLConfiguration() const;

  //---------------------- Server functions -----------------------//

  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  const QList<QPair<QHostAddress, quint16>> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   * @param host ip address of the client
   * @param ip port number of the client
   */
  void disconnectClient(QPair<QHostAddress, quint16> client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief Get the server QHostAddress and port
   */
  QPair<QHostAddress, quint16> getServer();

  //---------------------- Client functions -----------------------//

  /**
   * @brief Get the Server List object
   *
   * @return QList<QPair<QHostAddress, quint16>> List of servers
   */
  QList<QPair<QHostAddress, quint16>> getServerList();

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(QPair<QHostAddress, quint16> host);

  /**
   * @brief get the connected server address and port
   *
   * @return QPair<QHostAddress, quint16> address and port
   */
  QPair<QHostAddress, quint16> getConnectedServer();

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer(QPair<QHostAddress, quint16> host);
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
