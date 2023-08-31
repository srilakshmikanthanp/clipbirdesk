// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "client.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Verify Server
 */
bool Client::verifyCertificate(const QSslCertificate& certificate) {
  if (certificate.isNull() || certificate.subjectInfo(QSslCertificate::CommonName).isEmpty()) {
    return false;
  } else {
    return true;
  }
}

/**
 * @brief Process SSL Errors
 */
void Client::processSslErrors(const QList<QSslError>& errors) {
  for (auto error: errors) {
    if (error.error() == QSslError::SelfSignedCertificate && verifyCertificate(error.certificate())) {
      this->m_ssl_socket->ignoreSslErrors();
    } else {
      this->m_ssl_socket->abort();
      break;
    }
  }
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
      return get.rollbackTransaction();
    }

    toRead -= bytes;
  }

  // commit the transaction
  if (!get.commitTransaction()) {
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
  const auto signal_c = &QSslSocket::encrypted;
  const auto slot_c   = [=] {emit OnServerStatusChanged(true); };
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
  const auto slot_s   = &Client::processSslErrors;
  connect(m_ssl_socket, signal_s, this, slot_s);

  // disconnected signal to emit the signal for
  // server state changed
  const auto signal_d = &QSslSocket::disconnected;
  const auto slot_d   = [=] {emit OnServerStatusChanged(false); };
  connect(m_ssl_socket, signal_d, this, slot_d);
}

/**
 * @brief Set SSL configuration
 */
void Client::setSslConfiguration(QSslConfiguration config) {
  this->m_ssl_config = config;
}

/**
 * @brief get the SSL configuration
 */
QSslConfiguration Client::getSslConfiguration() const {
  return this->m_ssl_config;
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
  SyncingPacket packet = createPacket({
    SyncingPacket::PacketType::SyncPacket, items
  });

  // send the packet to the server
  this->sendPacket(packet);
}

/**
 * @brief Get the Server List object
 *
 * @return QList<QPair<QHostAddress, quint16>> List of servers
 */
QList<types::device::Device> Client::getServerList() const {
  return m_servers;
}

/**
 * @brief Connect to the server with the given host and port
 * number
 *
 * @param host Host address
 * @param port Port number
 */
void Client::connectToServer(types::device::Device server) {
  // if Discover Configuration is null the return
  if (this->m_ssl_config.isNull()) {
    throw std::runtime_error("SSL Config Config is not set");
  }

  // set the ssl Configuration
  m_ssl_socket->setSslConfiguration(m_ssl_config);

  // check if the socket is connected
  if (this->isConnected()) {
    this->disconnectFromServer();
  }

  // create the host address
  const auto host = server.ip.toString();
  const auto port = server.port;

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
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
  // check if the socket is connected else throw error
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  // peer server details
  auto addr = m_ssl_socket->peerAddress();
  auto port = m_ssl_socket->peerPort();
  auto cert = m_ssl_socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // return the host address and port number
  return { addr, port, name, cert };
}

/**
 * @brief Disconnect from the server
 */
void Client::disconnectFromServer() {
  m_ssl_socket->disconnectFromHost();
}

/**
 * @brief On server found function that That Called by the
 * discovery client when the server is found
 *
 * @param host Host address
 * @param port Port number
 */
void Client::onServiceAdded(QPair<QHostAddress, quint16> server) {
  // if Discover Configuration is null the return
  if (this->m_ssl_config.isNull()) {
    throw std::runtime_error("SSL Config Config is not set");
  }

  // get the server certificate from the host
  QSslSocket *sslSocket = new QSslSocket(this);

  // set config to the socket
  sslSocket->setSslConfiguration(m_ssl_config);

  // connect the signal to the lambda function
  connect(sslSocket, &QSslSocket::encrypted, [=]() {
    // get the certificate
    auto cert = sslSocket->peerCertificate();

    // if certificate is null or common name is empty
    if (cert.isNull() || cert.subjectInfo(QSslCertificate::CommonName).isEmpty()) {
      return;
    }

    // get the common name
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    // Get the Device Object
    auto device = types::device::Device {
      server.first, server.second, name, cert
    };

    // emit on server found
    emit OnServerFound(device);

    // add the server to the list
    m_servers.append(device);

    // emit the signal
    emit OnServerListChanged(getServerList());

    // disconnect the server
    sslSocket->disconnectFromHost();
  });

  // connect to ssl errors
  connect(sslSocket, &QSslSocket::sslErrors, [=]() {
    sslSocket->ignoreSslErrors();
  });

  // server details
  auto address = server.first.toString();
  auto port    = server.second;

  // connect to the server as encrypted
  sslSocket->connectToHostEncrypted(address, port);
}

/**
 * @brief On server removed function that That Called by the
 * discovery client when the server is removed
 */
void Client::onServiceRemoved(QPair<QHostAddress, quint16> server) {
  // matcher to get the Device from servers list
  auto matcher = [server](const types::device::Device& device) {
    return device.ip == server.first && device.port == server.second;
  };

  // start and end
  auto start = m_servers.begin();
  auto end   = m_servers.end();

  // Get the Device
  auto device = std::find_if(start, end, matcher);

  // if device is not found
  if (device == m_servers.end()) {
    return;
  }

  // emit server gone
  emit OnServerGone(*device);

  // remove the server from the list
  m_servers.erase(device);

  // emit the signal
  emit OnServerListChanged(getServerList());
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
