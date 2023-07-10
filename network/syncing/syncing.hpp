#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>

#include "discovery/discovery.hpp"
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

 private:
  /**
   * @brief Callback function that process pending
   * connections when the server is ready to accept
   */
  void processConnections() {
    while (m_ssl_server.hasPendingConnections()) {
      // TODO: Implement the function
    }
  }

 public:
  /**
   * @brief Construct a new Syncing Server object and
   * bind to any available port and any available
   * IP address
   *
   * @param parent Parent object
   */
  explicit SyncingServer(QObject *parent) : DiscoveryServer(parent) {
    // Connect the socket to the callback function that
    // process the connections when the socket is ready
    // to read so the listener can be notified
    const auto signal_u = &QSslServer::pendingConnectionAvailable;
    const auto slot_u = &SyncingServer::processConnections;
    QObject::connect(&m_ssl_server, signal_u, this, slot_u);

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
  virtual types::enums::IPType getIPType() const {
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
  virtual quint16 getPort() const {
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
  virtual QByteArray getIPAddress() const {
    // using toQHostIPv4Address from ipconv namespace
    using utility::functions::ipconv::toQByteArrayIPv4Address;

    // Address of the server
    const auto address = m_ssl_server.serverAddress();

    // return the IP address of the server
    return toQByteArrayIPv4Address(address);
  }

 public:  // functions
  /**
   * @brief Set the SSL Configuration for the SyncingServer
   *
   * @param config SSL Configuration
   */
  void setSSLConfiguration(const QSslConfiguration &config) {
    m_ssl_server.setSslConfiguration(config);
  }

  /**
   * @brief Get the SSL Configuration for the SyncingServer
   *
   * @return QSslConfiguration SSL Configuration
   */
  QSslConfiguration getSSLConfiguration() const {
    return m_ssl_server.sslConfiguration();
  }
};

/**
 * @brief Syncing client that syncs the clipboard data between
 * client and server
 */
class SyncingClient : public QObject {};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
