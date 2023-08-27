// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "client.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Process the Authentication Packet from the server
 *
 * @param packet Authentication
 */
void Client::processAuthentication(const packets::Authentication& packet) {
  if (packet.getAuthStatus() == types::enums::AuthStatus::AuthOkay) {
    emit OnServerAuthentication((this->m_is_authed = true));
  } else {
    emit OnServerAuthentication((this->m_is_authed = false));
  }
}

/**
 * @brief Process the packet that has been received
 * from the server and emit the signal
 *
 * @param packet Syncing packet
 */
void Client::processSyncingPacket(const packets::SyncingPacket& packet) {
  // Make the vector of QPair<QString, QByteArray>
  QVector<QPair<QString, QByteArray>> items;

  // Get the items from the packet
  for (auto item : packet.getItems()) {
    items.append({item.getMimeType().toStdString().c_str(), item.getPayload()});
  }

  // emit the signal
  emit OnSyncRequest(items);
}

/**
 * @brief Process the Invalid packet that has been received
 * from the server and emit the signal
 *
 * @param packet Invalid packet
 */
void Client::processInvalidPacket(const packets::InvalidRequest& packet) {
  qWarning() << (LOG(packet.getErrorMessage().toStdString()));
}

/**
 * @brief Process the packet that has been received
 * from the server
 */
void Client::processReadyRead() {
  // using fromQByteArray to parse the packet
  using utility::functions::fromQByteArray;

  // get the first four bytes of the packet
  QDataStream get(m_ssl_socket);

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
  auto toRead = packetLength - data.length();

  // resize the data
  data.resize(packetLength);

  // read the data from the socket
  while (toRead > 0) {
    auto start = data.data() + data.size() - toRead;
    auto bytes = get.readRawData(start, toRead);

    if (bytes == -1) {
      get.rollbackTransaction();
      return;
    }

    toRead -= bytes;
  }

  // commit the transaction
  if (!get.commitTransaction()) {
    return;
  }

  // try to parse the packet
  try {
    processAuthentication(fromQByteArray<packets::Authentication>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    qWarning() << (LOG(e.what()));
  } catch (const std::exception& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (...) {
    qWarning() << (LOG("Unknown Error"));
    return;
  }

  // try to parse the packet
  try {
    processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    qWarning() << (LOG(e.what()));
  } catch (const std::exception& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (...) {
    qWarning() << (LOG("Unknown Error"));
    return;
  }

  // try to parse the packet
  try {
    processInvalidPacket(fromQByteArray<packets::InvalidRequest>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    qWarning() << (LOG(e.what()));
  } catch (const std::exception& e) {
    qWarning() << (LOG(e.what()));
    return;
  } catch (...) {
    qWarning() << (LOG("Unknown Error"));
    return;
  }

  // if no packet is found
  qWarning() << (LOG("Unknown Packet Found"));
}

/**
 * @brief Process the SSL error and emit the signal
 *
 * @param errors List of SSL errors
 */
void Client::processSslError(const QList<QSslError>& errors) {
  for (auto& error : errors) {
    qWarning() << (LOG(error.errorString().toStdString()));
  }
}

/**
 * @brief Handle client connected
 */
void Client::handleConnected() {
  emit OnServerStatusChanged(true);
}

/**
 * @brief Handle client disconnected
 */
void Client::handleDisconnected() {
  emit OnServerStatusChanged(false);
}

/**
 * @brief Construct a new Syncing Client object
 * and connect the signals and slots and start
 * the timer and Service discovery
 *
 * @param th threshold
 * @param parent Parent
 */
Client::Client(QObject* parent) : service::mdnsBrowser(parent) {
  // connect the signal to emit the signal for
  // server state changed
  const auto signal_c = &QSslSocket::connected;
  const auto slot_c   = &Client::handleConnected;
  connect(m_ssl_socket, signal_c, this, slot_c);

  // connect the signals and slots for the socket
  // readyRead signal to process the packet
  const auto signal_r = &QSslSocket::readyRead;
  const auto slot_r   = &Client::processReadyRead;
  connect(m_ssl_socket, signal_r, this, slot_r);

  // connect the signals and slots for the socket
  // sslErrors signal to emit the signal for
  // OnErrorOccurred
  const auto signal_s = &QSslSocket::sslErrors;
  const auto slot_s   = &Client::processSslError;
  connect(m_ssl_socket, signal_s, this, slot_s);

  // disconnected signal to emit the signal for
  // server state changed
  const auto signal_d = &QSslSocket::disconnected;
  const auto slot_d   = &Client::handleDisconnected;
  connect(m_ssl_socket, signal_d, this, slot_d);

  // set the peer verify mode to none
  m_ssl_socket->setPeerVerifyMode(QSslSocket::VerifyNone);
}

/**
 * @brief Send the items to the server to sync the
 * clipboard data
 *
 * @param items QVector<QPair<QString, QByteArray>>
 */
void Client::syncItems(QVector<QPair<QString, QByteArray>> items) {
  // check if the socket is connected else throw error
  if (!m_ssl_socket->isOpen()) {
    throw std::runtime_error("Socket is not connected");
  }

  // using createPacket to create the packet
  using packets::SyncingPacket;
  using utility::functions::createPacket;

  // create the packet
  SyncingPacket packet = createPacket({SyncingPacket::PacketType::SyncPacket, items});

  // send the packet to the server
  this->sendPacket(packet);
}

/**
 * @brief Get the Server List object
 *
 * @return QList<QPair<QHostAddress, quint16>> List of servers
 */
QList<types::device::Device> Client::getServerList() const {
  // Host address and port number
  QList<types::device::Device> list;

  // iterate and add the server
  for (auto& server : m_servers) {
    list.append(server);
  }

  // return the list
  return list;
}

/**
 * @brief Connect to the server with the given host and port
 * number
 *
 * @param host Host address
 * @param port Port number
 */
void Client::connectToServer(types::device::Device client) {
  // on Encrypted lambda function to process
  const auto onEncrypted = [this]() {
    // create Authentication packet
    auto packet = utility::functions::createPacket({
        packets::Authentication::PacketType::AuthPacket,
        types::enums::AuthType::AuthReq,
        types::enums::AuthStatus::AuthStart,
    });

    // send the packet to the server
    this->sendPacket(packet);
  };

  // check if the SSL configuration is set
  if (m_ssl_socket->sslConfiguration().isNull()) {
    throw std::runtime_error("SSL Configuration is not set");
  }

  // check if the socket is connected
  if (m_ssl_socket->isOpen()) {
    this->disconnectFromServer();
  }

  // create the host address
  const auto host = client.ip.toString();
  const auto port = client.port;

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);

  // connect the signal to the lambda function
  connect(m_ssl_socket, &QSslSocket::encrypted, onEncrypted);
}

/**
 * @brief IS connected to the server
 */
bool Client::isConnected() const {
  return m_ssl_socket->state() == QAbstractSocket::ConnectedState;
}

/**
 * @brief Get the Connection Host and Port object
 * @return QPair<QHostAddress, quint16>
 */
types::device::Device Client::getConnectedServer() const {
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  // peer server details
  auto address = m_ssl_socket->peerAddress();
  auto port    = m_ssl_socket->peerPort();
  auto cert    = m_ssl_socket->peerCertificate();
  auto name    = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // return the host address and port number
  return { address, port, name, cert };
}


/**
 * @brief Disconnect from the server
 */
void Client::disconnectFromServer() {
  m_ssl_socket->disconnectFromHost();
}

/**
 * @brief Is client is authenticated
 */
bool Client::isAuthenticated() const {
  return m_is_authed;
}

/**
 * @brief Get the Authed Server object
 * @return QPair<QHostAddress, quint16>
 */
types::device::Device Client::getAuthedServer() const {
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  if (!this->m_is_authed) {
    throw std::runtime_error("Socket is not authenticated");
  }

  return this->getConnectedServer();
}

/**
 * @brief On server found function that That Called by the
 * discovery client when the server is found
 *
 * @param host Host address
 * @param port Port number
 */
void Client::onServiceAdded(QPair<QHostAddress, quint16> server) {
  // on Encrypted lambda function to process
  const auto onEncrypted = [this, server](QNetworkReply *reply) {
    // get the peer certificate
    auto cert = reply->sslConfiguration().peerCertificate();
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    // emit the signal for server found event
    emit OnServerFound({server.first, server.second, name, cert});

    // add the server to the list
    m_servers.append({server.first, server.second, name, cert});

    // emit the signal
    emit OnServerListChanged(getServerList());
  };

  // create the request
  auto access = new QNetworkAccessManager(this);

  // connect the signal to the lambda function
  connect(access, &QNetworkAccessManager::encrypted, onEncrypted);

  // send the request
  access->connectToHostEncrypted(server.first.toString(), server.second);
}

/**
 * @brief On server removed function that That Called by the
 * discovery client when the server is removed
 */
void Client::onServiceRemoved(QPair<QHostAddress, quint16> server) {
  // on Encrypted lambda function to process
  const auto onEncrypted = [this, server](QNetworkReply *reply) {
    // get the peer certificate
    auto cert = reply->sslConfiguration().peerCertificate();
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    // emit the signal for server found event
    emit OnServerGone({server.first, server.second, name, cert});

    // remove the server from the list
    m_servers.removeAll({server.first, server.second, name, cert});

    // emit the signal
    emit OnServerListChanged(getServerList());
  };

  // create the request
  auto access = new QNetworkAccessManager(this);

  // connect the signal to the lambda function
  connect(access, &QNetworkAccessManager::encrypted, onEncrypted);

  // send the request
  access->connectToHostEncrypted(server.first.toString(), server.second);
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
