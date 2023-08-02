// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "server.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Process the SyncingPacket from the client
 *
 * @param packet SyncingPacket
 */
void Server::processSyncingPacket(const packets::SyncingPacket &packet) {
  // Make the vector of QPair<QString, QByteArray>
  QVector<QPair<QString, QByteArray>> items;

  // Get the items from the packet
  for (auto item : packet.getItems()) {
    items.append({item.getMimeType().toStdString().c_str(), item.getPayload()});
  }

  // Notify the listeners to sync the data
  emit OnSyncRequest(items);}

/**
 * @brief Process the connections that are pending
 */
void Server::processConnections() {
  while (m_ssl_server->hasPendingConnections()) {
    // Get the client that has been connected
    auto client_tcp = (m_ssl_server->nextPendingConnection());

    // Convert the client to SSL client
    auto client_tls = qobject_cast<QSslSocket *>(client_tcp);

    // If the client is not SSL client then disconnect
    if (client_tls == nullptr) {
      const auto packType = packets::InvalidRequest::PacketType::RequestFailed;
      const auto code     = types::enums::ErrorCode::SSLError;
      const auto message  = "Client is not SSL client";

      using utility::functions::createPacket;
      this->sendPacket(client_tls, createPacket({packType, code, message}));
      client_tcp->disconnectFromHost();
    } else {
      // add the client to unauthenticated list
      m_un_authed_clients.append(client_tls);

      // get the peer address ans port
      const auto peerAddress = client_tls->peerAddress();
      const auto peerPort    = client_tls->peerPort();

      // emit the signal that new host is connected
      emit OnNewHostConnected({peerAddress, peerPort});
    }
  }
}

/**
 * @brief Callback function that process the ready
 * read from the client
 */
void Server::processReadyRead() {
  // Get the client that was ready to read
  auto client = qobject_cast<QSslSocket *>(sender());

  // using the fromQByteArray from namespace
  using utility::functions::createPacket;
  using utility::functions::fromQByteArray;

  // get the first four bytes of the packet
  QDataStream get(client);

  // QByteArray to store the data
  QByteArray data;

  /// uploader QDataStream
  QDataStream put(&data, QIODevice::WriteOnly);

  // start the transaction
  get.startTransaction();

  // get the packet length
  qint32 packetLength;

  // read the packet length
  get >> packetLength;

  // write the packet length
  put << packetLength;

  // infer the read size
  quint32 toRead = packetLength - data.size();

  // resize the data
  data.resize(packetLength);

  // read the data from the socket
  while (toRead > 0) {
    // Try to read the data
    auto start = data.data() + data.size() - toRead;
    auto bytes = get.readRawData(start, toRead);

    // if failed, then rollback
    if (bytes == -1) {
      get.rollbackTransaction();
      return;
    }

    // if success, then update
    toRead -= bytes;
  }

  // commit the transaction
  if (!get.commitTransaction()) {
    return;
  }

  // Deserialize the data to SyncingPacket
  try {
    this->processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    this->sendPacket(fromQByteArray<packets::SyncingPacket>(data), client);
    return;
  } catch (const types::except::MalformedPacket &e) {
    const auto type = packets::InvalidRequest::PacketType::RequestFailed;
    this->sendPacket(client, createPacket({type, e.getCode(), e.what()}));
    return;
  } catch (const types::except::NotThisPacket &e) {
    emit OnErrorOccurred(LOG(e.what()));
  } catch (const std::exception &e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (...) {
    emit OnErrorOccurred(LOG("Unknown Error"));
    return;
  }

  // if the packet is none of the above then send the invalid packet
  const auto type = packets::InvalidRequest::PacketType::RequestFailed;
  const auto code = types::enums::ErrorCode::InvalidPacket;
  const auto msg  = "Invalid Packet";
  this->sendPacket(client, createPacket({type, code, msg}));
}

/**
 * @brief Process the disconnection from the client
 */
void Server::processDisconnection() {
  // Get the client that was disconnected
  auto client = qobject_cast<QSslSocket *>(sender());

  // convert to QPair<QHostAddress, quint16>
  auto info   = QPair<QHostAddress, quint16>(client->peerAddress(), client->peerPort());

  // Notify the listeners that the client is disconnected
  emit OnCLientStateChanged(info, false);

  // Remove the client from the list of clients
  m_authed_clients.removeOne(client);

  // Notify the listeners that the client list is changed
  emit OnClientListChanged(getConnectedClientsList());
}

/**
 * @brief Called when the service is registered
 */
void Server::OnServiceRegistered() {
  emit OnServerStateChanged(true);
}

/**
 * @brief Construct a new Syncing Server object and
 * bind to any available port and any available
 * IP address
 *
 * @param config SSL configuration
 * @param parent Parent object
 */
Server::Server(QObject *p) : mDNSRegister(p) {
  // Connect the socket to the callback function that
  // process the connections when the socket is ready
  // to read so the listener can be notified
  const auto signal_c = &QSslServer::pendingConnectionAvailable;
  const auto slot_c   = &Server::processConnections;
  QObject::connect(m_ssl_server, signal_c, this, slot_c);

  // connect OnErrorOccurred to from base class
  const auto signal_e = &mDNSRegister::OnErrorOccurred;
  const auto slot_e   = &Server::OnErrorOccurred;
  QObject::connect(this, signal_e, this, slot_e);

  // Notify the listeners that the server is started
  const auto signal = &mDNSRegister::OnServiceRegistered;
  const auto slot   = &Server::OnServiceRegistered;
  QObject::connect(this, signal, this, slot);
}

/**
 * @brief Request the clients to sync the clipboard items
 *
 * @param data QVector<QPair<QString, QByteArray>>
 */
void Server::syncItems(QVector<QPair<QString, QByteArray>> items) {
  const auto packType = packets::SyncingPacket::PacketType::SyncPacket;
  this->sendPacket(utility::functions::createPacket({packType, items}));
}

/**
 * @brief Get the Clients that are connected to the server
 *
 * @return QList<QSslSocket*> List of clients
 */
QList<QPair<QHostAddress, quint16>> Server::getConnectedClientsList() const {
  QList<QPair<QHostAddress, quint16>> list;
  for (auto client : m_authed_clients) {
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
void Server::disconnectClient(QPair<QHostAddress, quint16> client) {
  for (auto c : m_authed_clients) {
    if (c->peerAddress() == client.first && c->peerPort() == client.second) {
      c->disconnectFromHost();
      return;
    }
  }
}

/**
 * @brief Disconnect the all the clients from the server
 */
void Server::disconnectAllClients() {
  for (auto client : m_authed_clients) client->disconnectFromHost();
}

/**
 * @brief Get the Server QHostAddress & Port
 *
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> Server::getServerInfo() const {
  return {m_ssl_server->serverAddress(), m_ssl_server->serverPort()};
}

/**
 * @brief Set the SSL Configuration object
 *
 * @param config SSL Configuration
 */
void Server::setSSLConfiguration(QSslConfiguration config) {
  m_ssl_server->setSslConfiguration(config);
}

/**
 * @brief Get the SSL Configuration object
 *
 * @return QSslConfiguration
 */
QSslConfiguration Server::getSSLConfiguration() const {
  return m_ssl_server->sslConfiguration();
}

/**
 * @brief The function that is called when the client is authenticated
 *
 * @param client the client that is currently processed
 */
void Server::authSuccess(const QPair<QHostAddress, quint16> &client) {
  // Matcher Lambda Function to find the client
  const auto matcher = [&client](QSslSocket *c) {
    return (c->peerAddress() == client.first) && (c->peerPort() == client.second);
  };

  // Get the iterator to the start and end of the list
  auto start      = m_un_authed_clients.begin();
  auto end        = m_un_authed_clients.end();

  // Get the client from the unauthenticated list and remove it
  auto client_itr = std::find_if(start, end, matcher);

  // If the client is not found then return from the function
  if (client_itr == m_un_authed_clients.end()) return;

  // Get the client from the iterator
  auto client_tls = *client_itr;

  // Remove the client from the unauthenticated list
  m_un_authed_clients.erase(client_itr);

  // Connect the client to the callback function that process
  // the disconnection when the client is disconnected
  // so the listener can be notified
  const auto signal_d = &QSslSocket::disconnected;
  const auto slot_d   = &Server::processDisconnection;
  QObject::connect(client_tls, signal_d, this, slot_d);

  // Connect the socket to the callback function that
  // process the ready read when the socket is ready
  // to read so the listener can be notified
  const auto signal_r = &QSslSocket::readyRead;
  const auto slot_r   = &Server::processReadyRead;
  QObject::connect(client_tls, signal_r, this, slot_r);

  // Peer info
  const auto peerAddress = client_tls->peerAddress();
  const auto peerPort    = client_tls->peerPort();

  // convert to QPair<QHostAddress, quint16>
  auto client_info       = QPair<QHostAddress, quint16>(peerAddress, peerPort);

  // Notify the listeners that the client is connected
  emit OnCLientStateChanged(client_info, true);

  // Add the client to the list of clients
  m_authed_clients.append(client_tls);

  // Notify the listeners that the client list is changed
  emit OnClientListChanged(getConnectedClientsList());

  // using the create packet from namespace
  using utility::functions::createPacket;

  // create the Authentication packet
  packets::Authentication packet = createPacket({
      packets::Authentication::PacketType::AuthStatus,
      types::enums::AuthStatus::AuthSuccess,
  });

  // send the packet to the client
  this->sendPacket(client_tls, packet);
}

/**
 * @brief The function that is called when the client is not authenticated
 *
 * @param client the client that is currently processed
 */
void Server::authFailed(const QPair<QHostAddress, quint16> &client) {
  // Matcher Lambda Function to find the client
  const auto matcher = [&client](QSslSocket *c) {
    return (c->peerAddress() == client.first) && (c->peerPort() == client.second);
  };

  // Get the iterator to the start and end of the list
  auto start      = m_un_authed_clients.begin();
  auto end        = m_un_authed_clients.end();

  // Get the client from the unauthenticated list and remove it
  auto client_itr = std::find_if(start, end, matcher);

  // If the client is not found then return from the function
  if (client_itr == m_un_authed_clients.end()) return;

  // Remove the client from the unauthenticated list
  m_un_authed_clients.erase(client_itr);

  // using the create packet from namespace
  using utility::functions::createPacket;

  // create the Authentication packet
  packets::Authentication packet = createPacket({
      packets::Authentication::PacketType::AuthStatus,
      types::enums::AuthStatus::AuthFailed,
  });

  // send the packet to the client
  this->sendPacket((*client_itr), packet);

  // disconnect and delete the client
  (*client_itr)->disconnectFromHost();

  // delete the client after the client is disconnected
  (*client_itr)->deleteLater();
}

/**
 * @brief Start the server
 */
void Server::startServer() {
  // check if the SSL configuration is set
  if (m_ssl_server->sslConfiguration().isNull()) {
    throw std::runtime_error("SSL Configuration is not set");
  }

  // start the server
  if (!m_ssl_server->listen()) {
    throw std::runtime_error("Failed to start the server");
  }

  // start the discovery server
  this->registerServiceAsync();
}

/**
 * @brief Stop the server
 */
void Server::stopServer() {
  // stop the discovery server
  this->unregisterService();

  // disconnect all the clients
  this->disconnectAllClients();

  // stop the server
  m_ssl_server->close();

  // Notify the listeners
  emit OnServerStateChanged(false);
}

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
types::enums::IPType Server::getIPType() const {
  return types::enums::IPType::IPv4;
}

/**
 * @brief Get the Port number of the Server it can be any port
 * number from 0 to 65535 but the port number should be greater than 1024
 * because the port number less than 1024 are reserved for the system
 * services
 *
 * @return types::Port Port number
 * @throw Any Exception If any error occurs
 */
quint16 Server::getPort() const {
  return m_ssl_server->serverPort();
}

/**
 * @brief Get the IP Address of the Server it can be IPv4 or
 * IPv6 if the IP type is IPv4 then the IP address is 4 bytes long if
 * the IP type is IPv6 then the IP address is 16 bytes long
 *
 * @return types::IPAddress IP address
 * @throw Any Exception If any error occurs
 */
QHostAddress Server::getIPAddress() const {
  return m_ssl_server->serverAddress();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
