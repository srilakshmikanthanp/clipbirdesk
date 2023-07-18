#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>
#include <QSslSocket>
#include <QVector>

#include "network/discovery/server/server.hpp"
#include "types/callback/callback.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing server that syncs the clipboard data between
 * the clients
 */
class Server : public discovery::Server {
 signals:  // signals
  /// @brief On client state changed
  void OnCLientStateChanged(QPair<QHostAddress, quint16>, bool connected);

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 signals:  // signals for this class
  /// @brief On Server State Changed
  void OnServerStateChanged(bool started);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnClientListChanged(QList<QPair<QHostAddress, quint16>> clients);

 signals:  // signals
  /// @brief On Sync Request
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Server)

 public:  // Authenticator Type

  /// @brief Authenticator
  using Authenticator = types::callback::Authenticator;

 private:  // members of the class

  /// @brief SSL server
  QSslServer* m_ssl_server = new QSslServer(this);

  /// @brief List of clients
  QList<QSslSocket*> m_clients;

 private:  // Authenticator Instance

  /// @brief Authenticator
  Authenticator m_authenticator = nullptr;

 private:  // some typedefs

  using MalformedPacket = types::except::MalformedPacket;

 private:  // member functions

  /**
   * @brief Create the packet and send it to the client
   *
   * @param client Client to send
   * @param packet Packet to send
   */
  template <typename Client, typename Packet>
  void sendPacket(Client* client, const Packet& pack) {
    client->write(utility::functions::toQByteArray(pack));
  }

  /**
   * @brief Create the packet and send it to all the
   * clients
   *
   * @param packet Packet to send
   */
  template <typename Packet>
  void sendPacket(const Packet& pack) {
    for (auto client : m_clients) {
      this->sendPacket(client, pack);
    }
  }

  /**
   * @brief Process the SyncingPacket from the client
   *
   * @param packet SyncingPacket
   */
  void processSyncingPacket(const packets::SyncingPacket& packet);

  /**
   * @brief Callback function that process the ready
   * read from the client
   */
  void processReadyRead();

  /**
   * @brief Process the connections that are pending
   */
  void processConnections();
  /**
   * @brief Process the disconnection from the client
   */
  void processDisconnection();

 public:  // constructors and destructors

  /**
   * @brief Construct a new Syncing Server object and
   * bind to any available port and any available
   * IP address
   *
   * @param config SSL configuration
   * @param parent Parent object
   */
  explicit Server(QObject* p = nullptr);

  /**
   * @brief Destroy the Syncing Server object
   */
  virtual ~Server() = default;

  /**
   * @brief Request the clients to sync the clipboard items
   *
   * @param data QVector<QPair<QString, QByteArray>>
   */
  void syncItems(QVector<QPair<QString, QByteArray>> items);

  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  QList<QPair<QHostAddress, quint16>> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   *
   * @param client Client to disconnect
   */
  void disconnectClient(QPair<QHostAddress, quint16> client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief Get the Server QHostAddress & Port
   *
   * @return QPair<QHostAddress, quint16>
   */
  QPair<QHostAddress, quint16> getServerInfo() const;

  /**
   * @brief Set the SSL Configuration object
   *
   * @param config SSL Configuration
   */
  void setSSLConfiguration(QSslConfiguration config);
  /**
   * @brief Get the SSL Configuration object
   *
   * @return QSslConfiguration
   */
  QSslConfiguration getSSLConfiguration() const;

  /**
   * @brief Set the Authenticator object
   *
   * @param auth Authenticator
   */
  void setAuthenticator(Authenticator auth);

  /**
   * @brief Get the Authenticator object
   *
   * @return Authenticator
   */
  Authenticator getAuthenticator() const;

  /**
   * @brief Start the server
   */
  void startServer() override;

  /**
   * @brief Stop the server
   */
  void stopServer() override;

 protected:  // override functions from the base class

  /**
   * @brief Get the IP Type of the Server it can be IPv4 or
   * IPv6 the IP type is used to determine the length of the IP address
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @note The IP Type used in Server is IPv4
   *
   * @return types::IPType IP type
   * @throw Any Exception If any error occurs
   */
  virtual types::enums::IPType getIPType() const override;

  /**
   * @brief Get the Port number of the Server it can be any port
   * number from 0 to 65535 but the port number should be greater than 1024
   * because the port number less than 1024 are reserved for the system
   * services
   *
   * @return types::Port Port number
   * @throw Any Exception If any error occurs
   */
  virtual quint16 getPort() const override;

  /**
   * @brief Get the IP Address of the Server it can be IPv4 or
   * IPv6 if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QHostAddress getIPAddress() const override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
