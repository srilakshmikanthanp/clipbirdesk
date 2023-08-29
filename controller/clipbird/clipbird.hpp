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
#include "store/storage.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class ClipBird : public QObject {
  //----------------------- client Signals -------------------------//

 signals:  // signals for this class
  /// @brief On Server List Changed (From Client)
  void OnServerListChanged(QList<types::device::Device> servers);

 signals:  // signals for this class
  /// @brief On Server Found  (From Client)
  void OnServerFound(types::device::Device server);

 signals:  // signals for this class
  /// @brief On Server Gone
  void OnServerGone(types::device::Device);

 signals:  // signals for this class
  /// @brief On Server state changed (From Client)
  void OnServerStatusChanged(bool isConnected);

 signals:  // signals for this class
  /// @brief On Server Auth Succeed (From Client)
  void OnServerAuthentication(bool isSuccessful);

  //----------------------- server Signals ------------------------//

 signals:  // signals
  /// @brief On client state changed (From Server)
  void OnCLientStateChanged(types::device::Device client, bool connected);

 signals:  // signals for this class
  /// @brief On New Host Connected
  void OnAuthRequested(types::device::Device client);

 signals:  // signals for this class
  /// @brief On Server state changed (From Server)
  void OnServerStateChanged(bool isStarted);

 signals:  // signals for this class
  /// @brief On Sync Request  (From Server)
  void OnClientListChanged(QList<types::device::Device> clients);

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

  /// @brief Handle On Server Authenticated (From client)
  void handleServerAuthentication(bool isConnected);

  /// @brief Handle On Server Disconnect (From client)
  void handleServerStatusChanged(bool status);

  /// @brief Handle the Server Found (From client)
  void handleServerFound(types::device::Device server);

  /// @brief Handle the Auth Request (From Server)
  void handleAuthRequest(types::device::Device client);

 public:  // Member functions

  /**
   * @brief Construct a new ClipBird object and manage
   * the clipboard, server and client
   *
   * @param board  clipboard that is managed
   * @param parent parent object
   */
  ClipBird(QSslConfiguration ssl_config, QObject *parent = nullptr);

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
  QList<types::device::Device> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   * @param host ip address of the client
   * @param ip port number of the client
   */
  void disconnectClient(const types::device::Device &client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief The function that is called when the client is authenticated
   *
   * @param client the client that is currently processed
   */
  void authSuccess(const types::device::Device &client);

  /**
   * @brief The function that is called when the client it not
   * authenticated
   *
   * @param client the client that is currently processed
   */
  void authFailed(const types::device::Device &client);

  /**
   * @brief Get the server QHostAddress and port
   */
  types::device::Device getServerInfo() const;

  //---------------------- Client functions -----------------------//

  /**
   * @brief Get the Server List object
   *
   * @return QList<types::device::Device> List of servers
   */
  QList<types::device::Device> getServerList() const;

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(const types::device::Device &host);

  /**
   * @brief Is Client Connected
   */
  bool isConnectedToServer();

  /**
   * @brief get the connected server address and port
   *
   * @return types::device::Device address and port
   */
  types::device::Device getConnectedServer() const;

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer(const types::device::Device &host);

  /**
   * @brief Is Client Authed
   */
  bool isClientAuthed();

  /**
   * @brief Get the Authed Server object
   * @return types::device::Device
   */
  types::device::Device getAuthedServer() const;

  //---------------------- General functions -----------------------//

  /**
   * @brief IS the Host is Lastly Server
   */
  bool isLastlyHostIsServer() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::controller
