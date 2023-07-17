// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "syncingserver.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {

/**
 * @brief Process the SyncingPacket from the client
 *
 * @param packet SyncingPacket
 */
void SyncingServer::processSyncingPacket(const packets::SyncingPacket &packet) {
  // Make the vector of QPair<QString, QByteArray>
  QVector<QPair<QString, QByteArray>> items;

// Get the items from the packet
#define STR(item) item.getMimeType().toStdString().c_str()
  for (auto item : packet.getItems()) {
    items.append({STR(item), item.getPayload()});
  }
#undef STR  // just used to avoid the long line

  // Notify the listeners to sync the data
  emit OnSyncRequest(items);

  // send the packet to all the clients
  this->sendPacket(packet);
}

/**
 * @brief Callback function that process the ready
 * read from the client
 */
void SyncingServer::processReadyRead() {
  // Get the client that was ready to read
  auto client = qobject_cast<QSslSocket *>(sender());

  // Get the data from the client
  const auto data = client->readAll();

  // using the fromQByteArray from namespace
  using utility::functions::createPacket;
  using utility::functions::fromQByteArray;

  // Deserialize the data to SyncingPacket
  try {
    this->processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const types::except::MalformedPacket &e) {
    const auto type = packets::InvalidRequest::PacketType::RequestFailed;
    this->sendPacket(client, createPacket({type, e.getCode(), e.what()}));
    return;
  } catch (const std::exception &e) {
    emit OnErrorOccurred(e.what());
    return;
  } catch (...) {
    emit OnErrorOccurred("Unknown Error");
    return;
  }
}

/**
 * @brief Process the connections that are pending
 */
void SyncingServer::processConnections() {
  while (m_ssl_server.hasPendingConnections()) {
    // Get the client that has been connected
    auto client_tcp = (m_ssl_server.nextPendingConnection());

    // Convert the client to SSL client
    auto client_tls = qobject_cast<QSslSocket *>(client_tcp);

    // using the createPacket from namespace
    using utility::functions::createPacket;

    // If the client is not SSL client then disconnect
    if (client_tls == nullptr) {
      const auto packType = packets::InvalidRequest::PacketType::RequestFailed;
      const auto code = types::enums::ErrorCode::SSLError;
      const auto message = "Client is not SSL client";
      this->sendPacket(client_tcp, createPacket({packType, code, message}));
      client_tcp->disconnectFromHost();
      continue;
    }

    // Authenticate the client
    if (!m_authenticator({client_tls->peerAddress(), client_tls->peerPort()})) {
      client_tls->disconnectFromHost();
      continue;
    }

    // Connect the client to the callback function that process
    // the disconnection when the client is disconnected
    // so the listener can be notified
    const auto signal_d = &QSslSocket::disconnected;
    const auto slot_d = &SyncingServer::processDisconnection;
    QObject::connect(client_tls, signal_d, this, slot_d);

    // Connect the socket to the callback function that
    // process the ready read when the socket is ready
    // to read so the listener can be notified
    const auto signal_r = &QSslSocket::readyRead;
    const auto slot_r = &SyncingServer::processReadyRead;
    QObject::connect(client_tls, signal_r, this, slot_r);

    // convert to QPair<QHostAddress, quint16>
    auto client_info = QPair<QHostAddress, quint16>(client_tls->peerAddress(),
                                                    client_tls->peerPort());

    // Notify the listeners that the client is connected
    emit OnCLientStateChanged(client_info, true);

    // Add the client to the list of clients
    m_clients.append(client_tls);

    // Notify the listeners that the client list is changed
    emit OnClientListChanged(getConnectedClientsList());
  }
}

/**
 * @brief Process the disconnection from the client
 */
void SyncingServer::processDisconnection() {
  // Get the client that was disconnected
  auto client = qobject_cast<QSslSocket *>(sender());

  // convert to QPair<QHostAddress, quint16>
  auto client_info =
      QPair<QHostAddress, quint16>(client->peerAddress(), client->peerPort());

  // Notify the listeners that the client is disconnected
  emit OnCLientStateChanged(client_info, false);

  // Remove the client from the list of clients
  m_clients.removeOne(client);

  // Notify the listeners that the client list is changed
  emit OnClientListChanged(getConnectedClientsList());
}

/**
 * @brief Construct a new Syncing Server object and
 * bind to any available port and any available
 * IP address
 *
 * @param config SSL configuration
 * @param parent Parent object
 */
SyncingServer::SyncingServer(QObject *p): DiscoveryServer(p) {
  // Connect the socket to the callback function that
  // process the connections when the socket is ready
  // to read so the listener can be notified
  const auto signal_c = &QSslServer::pendingConnectionAvailable;
  const auto slot_c = &SyncingServer::processConnections;
  QObject::connect(&m_ssl_server, signal_c, this, slot_c);
}

/**
 * @brief Request the clients to sync the clipboard items
 *
 * @param data QVector<QPair<QString, QByteArray>>
 */
void SyncingServer::syncItems(QVector<QPair<QString, QByteArray>> items) {
  const auto packType = packets::SyncingPacket::PacketType::SyncPacket;
  this->sendPacket(utility::functions::createPacket(packType, items));
}

/**
 * @brief Get the Clients that are connected to the server
 *
 * @return QList<QSslSocket*> List of clients
 */
QList<QPair<QHostAddress, quint16>> SyncingServer::getConnectedClientsList()
    const {
  QList<QPair<QHostAddress, quint16>> list;
  for (auto client : m_clients) {
    list.append({client->peerAddress(), client->peerPort()});
  }
  return list;
}

/**
 * @brief Disconnect the client from the server and delete
 * the client
 *
 * @param client Client to disconnect
 */
void SyncingServer::disconnectClient(QPair<QHostAddress, quint16> client) {
  for (auto c : m_clients) {
    if (c->peerAddress() == client.first && c->peerPort() == client.second) {
      c->disconnectFromHost();
      return;
    }
  }
}

/**
 * @brief Disconnect the all the clients from the server
 */
void SyncingServer::disconnectAllClients() {
  for (auto client : m_clients) client->disconnectFromHost();
}

/**
 * @brief Get the Server QHostAddress & Port
 *
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> SyncingServer::getServerInfo() const {
  return {m_ssl_server.serverAddress(), m_ssl_server.serverPort()};
}

/**
 * @brief Set the SSL Configuration object
 *
 * @param config SSL Configuration
 */
void SyncingServer::setSSLConfiguration(QSslConfiguration config) {
  m_ssl_server.setSslConfiguration(config);
}

/**
 * @brief Get the SSL Configuration object
 *
 * @return QSslConfiguration
 */
QSslConfiguration SyncingServer::getSSLConfiguration() const {
  return m_ssl_server.sslConfiguration();
}

/**
 * @brief Set the Authenticator object
 *
 * @param auth Authenticator
 */
void SyncingServer::setAuthenticator(Authenticator auth) {
  m_authenticator = auth;
}

/**
 * @brief Get the Authenticator object
 *
 * @return Authenticator
 */
SyncingServer::Authenticator SyncingServer::getAuthenticator() const {
  return m_authenticator;
}

/**
 * @brief Start the server
 */
void SyncingServer::startServer() {
  // check if the SSL configuration is set
  if (m_ssl_server.sslConfiguration().isNull()) {
    throw std::runtime_error("SSL Configuration is not set");
  }

  // check if the authenticator is set
  if (m_authenticator == nullptr) {
    throw std::runtime_error("Authenticator is not set");
  }

  // start the server
  if (!m_ssl_server.listen()) {
    throw std::runtime_error("Failed to start the server");
  }

  // start the discovery server
  DiscoveryServer::startServer();

  // Notify the listeners that the server is started
  emit OnServerStateChanged(true);
}

/**
 * @brief Stop the server
 */
void SyncingServer::stopServer() {
  // stop the discovery server
  DiscoveryServer::stopServer();

  // stop the server
  m_ssl_server.close();

  // Notify the listeners
  emit OnServerStateChanged(false);
}

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
types::enums::IPType SyncingServer::getIPType() const {
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
quint16 SyncingServer::getPort() const {
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
QHostAddress SyncingServer::getIPAddress() const {
  return m_ssl_server.serverAddress();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
