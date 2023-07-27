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
#include "network/syncing/client/client.hpp"
#include "network/syncing/server/server.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class ClipBird : public QObject {
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
  /// @brief On New Host Connected
  void OnNewHostConnected(QPair<QHostAddress, quint16> client);

 signals:  // signals for this class
  /// @brief On Server state changed (From Server)
  void OnServerStateChanged(bool isStarted);

 signals:  // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnClientListChanged(QList<QPair<QHostAddress, quint16>> clients);

 private:  // typedefs for this class

  using Server = network::syncing::Server;
  using Client = network::syncing::Client;

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  std::variant<Server, Client> m_host;
  QSslConfiguration m_sslConfig;
  clipboard::Clipboard m_clipboard;

 private:  // private slots

  /// @brief Handle On Server Status Changed (From client)
  void handleServerStatusChanged(bool isConnected);

 public:  // Member functions

  /**
   * @brief Construct a new ClipBird object and manage
   * the clipboard, server and client
   *
   * @param board  clipboard that is managed
   * @param parent parent object
   */
  ClipBird(QClipboard *board, QSslConfiguration config, QObject *parent = nullptr);

  /**
   * @brief Destroy the ClipBird object
   */
  virtual ~ClipBird() = default;

  //---------------------- public slots ----------------------//

  /**
   * @brief set the host as server and start listening
   * to accept the client
   */
  void setCurrentHostAsServer();

  /**
   * @brief set the host as client
   */
  void setCurrentHostAsClient();

  //---------------------- Server functions -----------------------//

  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  QList<QPair<QHostAddress, quint16>> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   * @param host ip address of the client
   * @param ip port number of the client
   */
  void disconnectClient(const QPair<QHostAddress, quint16> &client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief The function that is called when the client is authenticated
   *
   * @param client the client that is currently processed
   */
  void authSuccess(const QPair<QHostAddress, quint16> &client);

  /**
   * @brief The function that is called when the client it not
   * authenticated
   *
   * @param client the client that is currently processed
   */
  void authFailed(const QPair<QHostAddress, quint16> &client);

  /**
   * @brief Get the server QHostAddress and port
   */
  QPair<QHostAddress, quint16> getServerInfo() const;

  //---------------------- Client functions -----------------------//

  /**
   * @brief Get the Server List object
   *
   * @return QList<QPair<QHostAddress, quint16>> List of servers
   */
  QList<QPair<QHostAddress, quint16>> getServerList() const;

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(const QPair<QHostAddress, quint16> &host);

  /**
   * @brief get the connected server address and port
   *
   * @return QPair<QHostAddress, quint16> address and port
   */
  QPair<QHostAddress, quint16> getConnectedServer() const;

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer(const QPair<QHostAddress, quint16> &host);
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
