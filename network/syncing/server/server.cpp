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
  emit OnSyncRequest(items);
}

/**
 * @brief Process SSL Errors
 */
void Server::processSslErrors(QSslSocket *socket, const QList<QSslError>& errors) {
  for (auto error : errors) {
    // if error not not verified certificate
    if (error.error() == QSslError::SelfSignedCertificate) {
      // peer info of the client
      const auto peerAddress = socket->peerAddress();
      const auto peerPort    = socket->peerPort();
      const auto certificate = socket->peerCertificate();

      // if certificate is null or common name is empty
      if (certificate.isNull() || certificate.subjectInfo(QSslCertificate::CommonName).isEmpty()) {
        continue;
      }

      // create the device
      auto device = types::device::Device {
        peerAddress,
        peerPort,
        certificate.subjectInfo(QSslCertificate::CommonName).constFirst(),
        certificate,
      };

      // if authenticator is not set then ignore the error
      if (m_authenticator == nullptr) {
        throw std::runtime_error("Authenticator is not set");
      }

      // if authenticator returns true then ignore the error
      if (m_authenticator(device)) {
        socket->ignoreSslErrors(QList<QSslError>{error});
      }
    }
  }
}

/**
 * @brief Process the connections that are pending
 */
void Server::processPendingConnections() {
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
      // Connect the client to the callback function that process
      // the disconnection when the client is disconnected
      const auto signal_d = &QSslSocket::disconnected;
      const auto slot_d   = &Server::processDisconnection;
      QObject::connect(client_tls, signal_d, this, slot_d);

      // Connect the socket to the callback function that
      // process the ready read when the socket is ready
      const auto signal_r = &QSslSocket::readyRead;
      const auto slot_r   = &Server::processReadyRead;
      QObject::connect(client_tls, signal_r, this, slot_r);

      if (client_tls->peerCertificate().isNull()) {
        throw std::runtime_error("Client Certificate is not set");
      }

      // Add the client to the list of unauthenticated clients
      m_authed_clients.append(client_tls);
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

  // if not authenticated then return
  if (!m_authed_clients.contains(client)) return;

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
      return get.rollbackTransaction();
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
    qWarning() << (LOG(e.what()));
  } catch (const std::exception &e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (...) {
    qWarning() << (LOG("Unknown Error"));
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

  // if not in authenticated list
  if (!m_authed_clients.contains(client)) return;

  // peer info of the client
  const auto peerAddress = client->peerAddress();
  const auto peerPort    = client->peerPort();
  const auto certificate = client->peerCertificate();

  // create the device
  auto device = types::device::Device {
    peerAddress,
    peerPort,
    certificate.subjectInfo(QSslCertificate::CommonName).constFirst(),
    certificate,
  };

  // Notify the listeners that the client is disconnected
  emit OnCLientStateChanged(device, false);

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
Server::Server(QObject *parent) : service::mdnsRegister(parent) {
  // Connect the socket to the callback function that
  // process the connections when the socket is ready
  // to read so the listener can be notified
  const auto signal_c = &QSslServer::pendingConnectionAvailable;
  const auto slot_c   = &Server::processPendingConnections;
  QObject::connect(m_ssl_server, signal_c, this, slot_c);

  // Notify the listeners that the server is started
  const auto signal = &service::mdnsRegister::OnServiceRegistered;
  const auto slot   = &Server::OnServiceRegistered;
  QObject::connect(this, signal, this, slot);

  // Connect the socket to the callback function that
  // process the SSL errors
  const auto signal_e = &QSslServer::sslErrors;
  const auto slot_e   = &Server::processSslErrors;
  QObject::connect(m_ssl_server, signal_e, this, slot_e);
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
QList<types::device::Device> Server::getConnectedClientsList() const {
  // List of clients that are connected
  QList<types::device::Device> list;

  for (auto client : m_authed_clients) {
    // Peer info of the client that is connected
    auto addr = client->peerAddress();
    auto port = client->peerPort();
    auto cert = client->peerCertificate();
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    // add the device to the list
    list.append({addr, port, name, cert});
  }

  // return the list
  return list;
}

/**
 * @brief Disconnect the client from the server and delete
 * the client
 *
 * @param client Client to disconnect
 */
void Server::disconnectClient(types::device::Device client) {
  // matcher lambda function to find the client
  const auto matcher = [&client](QSslSocket *c) {
    return (c->peerAddress() == client.ip) && (c->peerPort() == client.port);
  };

  // find the client from the list of clients
  for (auto c : m_authed_clients) {
    if (matcher(c)) {
      return c->disconnectFromHost();
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
 * @return types::device::Device
 */
types::device::Device Server::getServerInfo() const {
  // server information
  auto address = m_ssl_server->serverAddress();
  auto port = m_ssl_server->serverPort();
  auto cert = m_ssl_server->sslConfiguration().localCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // Return info
  return { address, port, name, cert };
}

/**
 * @brief Set the SSL Configuration object
 *
 * @param config SSL Configuration
 */
void Server::setSslConfiguration(QSslConfiguration config) {
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

  // log port
  std::cout << "Server Started on Port: " + QString::number(m_ssl_server->serverPort()).toStdString();

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
 * @brief Set the Authenticator object
 */
void Server::setAuthenticator(types::callable::Authenticator auth) {
  this->m_authenticator = auth;
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
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
