// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "server.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Process the connections that are pending
 */
void Server::processPendingConnections() {
  while (m_server->hasPendingConnections()) {
    // Get the client that has been connected
    auto client = qobject_cast<QSslSocket *>(m_server->nextPendingConnection());

    // get the device info of the client
    auto addr = client->peerAddress();
    auto port = client->peerPort();
    auto cert = client->peerCertificate();
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    // Connect the disconnected signal to the processDisconnection function
    const auto signal_d = &QSslSocket::disconnected;
    const auto slot_d   = &Server::processDisconnection;
    QObject::connect(client, signal_d, this, slot_d);

    // create device object of the client
    auto device = types::device::Device {
      addr, port, name
    };

    // add to un authenticated list
    m_unauthenticatedClients.append(client);

    // Add the client to the list of authenticated clients
    if (!client->sslHandshakeErrors().isEmpty()) {
      emit OnAuthRequest(device);
    } else {
      this->authSuccess(device);
    }
  }
}

/**
 * @brief Process SSL Errors
 */
void Server::processSslErrors(QSslSocket *socket, const QList<QSslError>& errors) {
  // List of ignored SslErrors
  QList<QSslError::SslError> ignoredErrors;

  // Ignore the errors
  ignoredErrors.append(QSslError::SelfSignedCertificate);

  // make copy of errors
  auto errorsCopy = errors;

  // remove all the ignored errors
  auto itr = std::remove_if(errorsCopy.begin(), errorsCopy.end(), [&](auto error) {
    return ignoredErrors.contains(error.error());
  });

  // remove the ignored errors
  errorsCopy.erase(itr, errorsCopy.end());

  // log the errors for debugging
  for (auto error : errors) {
    qWarning() << (LOG(std::to_string(error.error()) + " : " + error.errorString().toStdString()));
  }

  // if errorsCopy is not empty
  if (!errorsCopy.isEmpty()) {
    return socket->abort();
  }

  // peer info of the client that is connected
  auto addr = socket->peerAddress();
  auto port = socket->peerPort();
  auto cert = socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName);

  // if certificate is null or common name is empty
  if (cert.isNull() || name.isEmpty()) {
    return socket->abort();
  }

  // just ignore wait for user
  socket->ignoreSslErrors();
}

/**
 * @brief Process the disconnection from the client
 */
void Server::processDisconnection() {
  // Get the client that was disconnected
  auto client = qobject_cast<QSslSocket *>(sender());

  // if in unauthenticated list
  if (m_unauthenticatedClients.removeOne(client)) {
    return;
  }

  // if not in authenticated list
  if (!m_clients.contains(client)) return;

  // peer information of the client
  auto addr = client->peerAddress();
  auto port = client->peerPort();
  auto cert = client->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // create the device
  auto device = types::device::Device {
    addr, port, name
  };

  // Remove the client from the list of clients
  m_clients.removeOne(client);

  // Notify the listeners that the client is disconnected
  emit OnCLientStateChanged(device, false);

  // get connected client list
  auto list = getConnectedClientsList();

  // Notify the listeners that the client list is changed
  emit OnClientListChanged(list);
}

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

  // get the Sender of the packet
  auto client = qobject_cast<QSslSocket *>(sender());

  // send the response packet
  this->sendPacket(packet, client);
}

/**
 * @brief Callback function that process the ready
 * read from the client
 */
void Server::processReadyRead() {
  // Get the client that was ready to read
  auto client = qobject_cast<QSslSocket *>(sender());

  // if not authenticated then return
  if (!m_clients.contains(client)) return;

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
    auto start = data.data() + data.size() - toRead;
    auto bytes = get.readRawData(start, toRead);

    if (bytes == -1) {
      return get.rollbackTransaction();
    }

    toRead -= bytes;
  }

  // commit the transaction
  if (!get.commitTransaction()) {
    return;
  }

  // Deserialize the data to SyncingPacket
  try {
    this->processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
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
  QObject::connect(m_server, signal_c, this, slot_c);

  // Notify the listeners that the server is started
  const auto signal = &service::mdnsRegister::OnServiceRegistered;
  const auto slot   = [=] { emit OnServerStateChanged(true); };
  QObject::connect(this, signal, this, slot);

  // Connect the socket to the callback function that
  // process the SSL errors
  const auto signal_e = &QSslServer::sslErrors;
  const auto slot_e   = &Server::processSslErrors;
  QObject::connect(m_server, signal_e, this, slot_e);
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

  // iterate through the list of clients
  for (auto c : m_clients) {
    auto name = c->peerCertificate().subjectInfo(QSslCertificate::CommonName).constFirst();
    list.append({c->peerAddress(), c->peerPort(), name});
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
  for (auto c : m_clients) {
    if (matcher(c)) {
      return c->disconnectFromHost();
    }
  }
}

/**
 * @brief Disconnect the all the clients from the server
 */
void Server::disconnectAllClients() {
  for (auto client : m_clients) client->disconnectFromHost();
}

/**
 * @brief Get the Server QHostAddress & Port
 *
 * @return types::device::Device
 */
types::device::Device Server::getServerInfo() const {
  auto cert = m_server->sslConfiguration().localCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();
  return { m_server->serverAddress(), m_server->serverPort(), name };
}

/**
 * @brief Set the SSL Configuration object
 *
 * @param config SSL Configuration
 */
void Server::setSslConfiguration(QSslConfiguration config) {
  m_server->setSslConfiguration(config);
}

/**
 * @brief Get the SSL Configuration object
 *
 * @return QSslConfiguration
 */
QSslConfiguration Server::getSSLConfiguration() const {
  return m_server->sslConfiguration();
}

/**
 * @brief Start the server
 */
void Server::startServer() {
  // check if the SSL configuration is set
  if (m_server->sslConfiguration().isNull()) {
    throw std::runtime_error("SSL Configuration is not set");
  }

  // start the server
  if (!m_server->listen()) {
    throw std::runtime_error("Failed to start the server");
  }

  // get the port
  auto port = std::to_string(m_server->serverPort());

  // log port
  qInfo() << (LOG("Server started at port: " + port));

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
  m_server->close();

  // Notify the listeners
  emit OnServerStateChanged(false);
}

/**
 * @brief Get the Dei=vice Certificate
 */
QSslCertificate Server::getClientCert(types::device::Device device) const {
  // matcher lambda function to find the client
  const auto matcher = [&device](QSslSocket *c) {
    return (c->peerAddress() == device.ip) && (c->peerPort() == device.port);
  };

  // find the client from the list of clients
  for (auto c : m_clients) {
    if (matcher(c)) {
      return c->peerCertificate();
    }
  }

  // if not found
  throw std::runtime_error("Client not found");
}

/**
 * @brief The function that is called when the client is authenticated
 *
 * @param client the client that is currently processed
 */
void Server::authSuccess(types::device::Device device) {
  // Matcher Lambda Function to find the client
  const auto matcher = [&device](QSslSocket *c) {
    return (c->peerAddress() == device.ip) && (c->peerPort() == device.port);
  };

  // Get the iterator to the start and end of the list
  auto start = m_unauthenticatedClients.begin();
  auto end   = m_unauthenticatedClients.end();
  auto itr   = std::find_if(start, end, matcher);

  // If the client is not found then return from the function
  if (itr == m_unauthenticatedClients.end()) return;

  // Get the client from the iterator
  auto client = *itr;

  // Connect the readyRead signal to the processReadyRead function
  const auto signal_r = &QSslSocket::readyRead;
  const auto slot_r   = &Server::processReadyRead;
  QObject::connect(client, signal_r, this, slot_r);

  // Remove the client from the unauthenticated list
  m_unauthenticatedClients.erase(itr);

  // Add the client to the list of clients
  m_clients.append(client);

  // Notify the listeners that the client is connected
  emit OnCLientStateChanged(device, true);

  // Notify the listeners that the client list is changed
  emit OnClientListChanged(getConnectedClientsList());

  // create the Authentication packet
  auto packet = utility::functions::createPacket({
    packets::Authentication::PacketType::AuthStatus,
    types::enums::AuthStatus::AuthOkay,
  });

  // send the packet to the client
  this->sendPacket(client, packet);
}

/**
 * @brief The function that is called when the client is not authenticated
 *
 * @param client the client that is currently processed
 */
void Server::authFailed(types::device::Device device) {
  // Matcher Lambda Function to find the client
  const auto matcher = [&device](QSslSocket *c) {
    return (c->peerAddress() == device.ip) && (c->peerPort() == device.port);
  };

  // Get the iterator to the start and end of the list
  auto start = m_unauthenticatedClients.begin();
  auto end   = m_unauthenticatedClients.end();
  auto itr   = std::find_if(start, end, matcher);

  // If the client is not found then return from the function
  if (itr == m_unauthenticatedClients.end()) return;

  // Remove the client from the unauthenticated list
  m_unauthenticatedClients.erase(itr);

  // create the Authentication packet
  auto packet = utility::functions::createPacket({
    packets::Authentication::PacketType::AuthStatus,
    types::enums::AuthStatus::AuthFail,
  });

  // send the packet to the client
  this->sendPacket((*itr), packet);

  // disconnect and delete the client
  (*itr)->disconnectFromHost();
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
  return m_server->serverPort();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
