#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>

#include "network/discovery/DiscoveryServer.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing server that syncs the clipboard data between
 * the clients
 */
class SyncingServer : public discovery::DiscoveryServer {
 private:
  /// @brief SSL server
  QSslServer m_ssl_server = QSslServer(this);

  /// @brief List of clients
  QList<QSslSocket *> m_clients;

 public:  // listeners
  /// @brief On client state changed
  using OnCLientStateChanged = void (*)(QSslSocket *client, bool connected);

 private:
  /// @brief List of listeners
  QList<OnCLientStateChanged> m_listeners;

 public:  // Authenticator Type
  /// @brief Authenticator
  using Authenticator = bool (*)(QSslSocket *client);

 private: // Authenticator Instance
  /// @brief Authenticator
  Authenticator m_authenticator = nullptr;

 private:
  /**
   * @brief Notify the listeners that the client state
   * has been changed
   *
   * @param client Client
   * @param connected Connected or not
   */
  void notifyListeners(QSslSocket *client, bool connected) {
    for (auto listener : m_listeners) {
      listener(client, connected);
    }
  }

  /**
   * @brief Callback function that process the client
   * disconnection
   */
  void processDisconnection() {
    // Get the client that has been disconnected
    auto client = qobject_cast<QSslSocket *>(sender());

    // Remove the client from the list of clients
    m_clients.removeOne(client);

    // Delete the client
    client->deleteLater();

    // Notify the listeners client state
    notifyListeners(client, false);
  }

  /**
   * @brief Callback function that process pending
   * connections when the server is ready to accept
   */
  void processConnections() {
    while (m_ssl_server.hasPendingConnections()) {
      // Get the client that has been connected
      auto client = qobject_cast<QSslSocket *>(m_ssl_server.nextPendingConnection());

      // Authenticate the client
      if (!m_authenticator(client)) {
        // Disconnect the client
        client->disconnectFromHost();

        // Delete the client
        client->deleteLater();

        // Move to the next client
        continue; // skip the client
      }

      // Connect the client to the callback function that process
      // the disconnection when the client is disconnected
      // so the listener can be notified
      const auto signal_d = &QSslSocket::disconnected;
      const auto slot_d = &SyncingServer::processDisconnection;
      QObject::connect(client, signal_d, this, slot_d);

      // Add the client to the list of clients
      m_clients.append(client);

      // Notify the listeners that the client has been connected
      notifyListeners(client, true);
    }
  }

 public:
  /**
   * @brief Construct a new Syncing Server object and
   * bind to any available port and any available
   * IP address
   *
   * @param config SSL configuration
   * @param parent Parent object
   */
  explicit SyncingServer(const QSslConfiguration &config, Authenticator authenticator, QObject *parent = nullptr)
    : DiscoveryServer(parent) {
    // Connect the socket to the callback function that
    // process the connections when the socket is ready
    // to read so the listener can be notified
    const auto signal_c = &QSslServer::pendingConnectionAvailable;
    const auto slot_c = &SyncingServer::processConnections;
    QObject::connect(&m_ssl_server, signal_c, this, slot_c);

    // set the configuration For the SSL server
    m_ssl_server.setSslConfiguration(config);

    // Set the authenticator
    m_authenticator = authenticator;

    // bind the server to any available port and any available
    // IP address and start listening for connections
    m_ssl_server.listen(QHostAddress::Any);
  }

  /**
   * @brief Destroy the Syncing Server object
   */
  virtual ~SyncingServer() = default;

 protected:  // functions
  /**
   * @brief Get the IP Type of the SyncingServer it can be IPv4 or
   * IPv6 the IP type is used to determine the length of the IP address
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @note The IP Type used in SyncingServer is IPv4
   *
   * @return types::IPType IP type
   * @throw Any Exception If any error occurs
   */
  virtual types::enums::IPType getIPType() const override {
    return types::enums::IPType::IPv4;
  }

  /**
   * @brief Get the Port number of the SyncingServer it can be any port
   * number from 0 to 65535 but the port number should be greater than 1024
   * because the port number less than 1024 are reserved for the system
   * services
   *
   * @return types::Port Port number
   * @throw Any Exception If any error occurs
   */
  virtual quint16 getPort() const override {
    return m_ssl_server.serverPort();
  }

  /**
   * @brief Get the IP Address of the SyncingServer it can be IPv4 or
   * IPv6 if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QByteArray getIPAddress() const override {
    // using toIPV4QHostAddress from ipconv namespace
    using utility::functions::toIPV4QByteArray;

    // Address of the server
    const auto address = m_ssl_server.serverAddress();

    // return the IP address of the server
    return toIPV4QByteArray(address);
  }

 public:
  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  QList<QSslSocket *> getClients() const {
    return m_clients;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
