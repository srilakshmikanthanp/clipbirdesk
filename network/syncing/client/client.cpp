// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "client.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Verify Server
 */
bool Client::verifyCert(const QSslCertificate& certificate) {
  // common name of the server
  auto name = certificate.subjectInfo(QSslCertificate::CommonName);

  // check the basic conditions
  if (certificate.isNull() || name.isEmpty()) {
    return false;
  } else {
    return true;
  }
}

/**
 * @brief Process SSL Errors
 */
void Client::processSslErrors(const QList<QSslError>& errors) {
  // List of ignored SslErrors
  QList<QSslError::SslError> ignoredErrors;

  // Ignore the errors
  ignoredErrors.append(QSslError::SelfSignedCertificate);
  ignoredErrors.append(QSslError::HostNameMismatch);
  ignoredErrors.append(QSslError::CertificateUntrusted);

  // make copy of errors
  auto errorsCopy = errors;

  // remove all the ignored errors
  auto itr = std::remove_if(errorsCopy.begin(), errorsCopy.end(), [&](auto error) {
    return ignoredErrors.contains(error.error());
  });

  // remove the ignored errors
  errorsCopy.erase(itr, errorsCopy.end());

  // log the errors
  for (auto error : errorsCopy) {
    qWarning() << (LOG(std::to_string(error.error()) + " : " + error.errorString().toStdString()));
  }

  // if errorsCopy is not empty
  if (!errorsCopy.isEmpty()) {
    return this->m_ssl_socket->abort();
  }

  // get certificate
  const auto certificate = this->m_ssl_socket->peerCertificate();

  // if any error occurred
  if (errors.length() > 0 && !this->verifyCert(certificate)) {
    return this->m_ssl_socket->abort();
  }

  // ignore the errors
  this->m_ssl_socket->ignoreSslErrors();
}

/**
 * @brief Process Ssl Error Secured
 */
void Client::processSslErrorsSecured(const QList<QSslError>& errors) {
  // List of ignored SslErrors
  QList<QSslError::SslError> ignoredErrors;

  // Ignore the errors
  ignoredErrors.append(QSslError::HostNameMismatch);

  // make copy of errors
  auto errorsCopy = errors;

  // remove all the ignored errors
  auto itr = std::remove_if(errorsCopy.begin(), errorsCopy.end(), [&](auto error) {
    return ignoredErrors.contains(error.error());
  });

  // remove the ignored errors
  errorsCopy.erase(itr, errorsCopy.end());

  // log the errors
  for (auto error : errorsCopy) {
    qWarning() << (LOG(std::to_string(error.error()).c_str()));
  }

  // if errors are not empty abort the connection
  if (!errorsCopy.isEmpty()) {
    return this->m_ssl_socket->abort();
  }

  // ignore the ssl errors
  this->m_ssl_socket->ignoreSslErrors();
}

/**
 * @brief Process the Authentication Packet from the server
 *
 * @param packet Authentication
 */
void Client::processAuthentication(const packets::Authentication& packet) {
  if (packet.getAuthStatus() == types::enums::AuthStatus::AuthOkay) {
    emit OnServerStatusChanged(true);
  }
}

/**
 * @brief Process the Invalid packet that has been received
 * from the server and emit the signal
 *
 * @param packet Invalid packet
 */
void Client::processInvalidPacket(const packets::InvalidRequest& packet) {
  emit OnInvalidPacket(QString("Error %1: %2").arg(packet.getErrorCode()).arg(packet.getErrorMessage()));
}

/**
 * @brief Precess the PingPacket from the client
 *
 * @param packet PingPacket
 */
void Client::processPingPacket(const packets::PingPacket &packet) {
  // using PingPacket Params
  using utility::functions::params::PingPacketParams;

  // using Ping Packet
  using packets::PingPacket;

  // if it is pong then ignore
  if (packet.getPingType() == types::enums::PingType::Pong) {
    qInfo() << (LOG("Pong Received")); return;
  }

  // create the PingPacket
  auto pingPacket = utility::functions::createPacket(PingPacketParams{
    PingPacket::PacketType::PingPong,
    types::enums::PingType::Pong
  });

  // send the packet to the client
  this->sendPacket(pingPacket);
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
  for (auto i : packet.getItems()) {
    if (i.getPayloadLength()) items.append({i.getMimeType().toStdString().c_str(), i.getPayload()});;
  }

  // is empty list
  if (items.isEmpty()) return;

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
    processPingPacket(fromQByteArray<packets::PingPacket>(data));
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
  // connect the signals and slots for the errorOccurred
  const auto signal_e = &QSslSocket::errorOccurred;
  const auto slot_e   = [=]{ this->OnConnectionError(this->m_ssl_socket->errorString()); };
  connect(m_ssl_socket, signal_e, this, slot_e);

  // connect the signals and slots for the socket
  // readyRead signal to process the packet
  const auto signal_r = &QSslSocket::readyRead;
  const auto slot_r   = &Client::processReadyRead;
  connect(m_ssl_socket, signal_r, this, slot_r);

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
  if (this->m_ssl_socket->state() == QAbstractSocket::ConnectedState) {
    this->m_ssl_socket->abort();
  }

  // disconnect the signals and slots for the socket
  const auto signal_ss = &QSslSocket::sslErrors;
  const auto slot_ss   = &Client::processSslErrorsSecured;
  disconnect(m_ssl_socket, signal_ss, this, slot_ss);

  // connect the signals and slots for the socket
  const auto signal_su = &QSslSocket::sslErrors;
  const auto slot_su   = &Client::processSslErrors;
  connect(m_ssl_socket, signal_su, this, slot_su);

  // create the host address
  const auto host = server.ip.toString();
  const auto port = server.port;

  // log host and port
  qInfo() << (LOG("Connecting to server: " + host.toStdString() + ":" + std::to_string(port)));

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
}

/**
 * @brief Connect to server Secured
 */
void Client::connectToServerSecured(types::device::Device server) {
  // if Discover Configuration is null the return
  if (this->m_ssl_config.isNull()) {
    throw std::runtime_error("SSL Config Config is not set");
  }

  // set the ssl Configuration
  m_ssl_socket->setSslConfiguration(m_ssl_config);

  // check if the socket is connected
  if (this->m_ssl_socket->state() == QAbstractSocket::ConnectedState) {
    this->m_ssl_socket->abort();
  }

  // disconnect the signals and slots for the socket
  const auto signal_su = &QSslSocket::sslErrors;
  const auto slot_su   = &Client::processSslErrors;
  disconnect(m_ssl_socket, signal_su, this, slot_su);

  // connect the signals and slots for the socket
  const auto signal_ss = &QSslSocket::sslErrors;
  const auto slot_ss   = &Client::processSslErrorsSecured;
  connect(m_ssl_socket, signal_ss, this, slot_ss);

  // create the host address
  const auto host = server.ip.toString();
  const auto port = server.port;

  // log host and port
  qInfo() << (LOG("Connecting to server: " + host.toStdString() + ":" + std::to_string(port)));

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
}

/**
 * @brief Get the Connection Host and Port object
 * @return QPair<QHostAddress, quint16>
 */
std::optional<types::device::Device> Client::getConnectedServer() const {
  // check if the socket is connected else throw error
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    return std::optional<types::device::Device>();
  }

  // peer server details
  auto addr = m_ssl_socket->peerAddress();
  auto port = m_ssl_socket->peerPort();
  auto cert = m_ssl_socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // return the host address and port number
  return std::optional<types::device::Device>({addr, port, name});
}

/**
 * @brief Disconnect from the server
 */
void Client::disconnectFromServer() {
  m_ssl_socket->disconnectFromHost();
}

/**
 * @brief Get the Server Certificate
 */
QSslCertificate Client::getConnectedServerCertificate() const{
  // if not connected throw error
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  // return the server certificate
  return m_ssl_socket->peerCertificate();
}

/**
 * @brief On server found function that That Called by the
 * discovery client when the server is found
 *
 * @param host Host address
 * @param port Port number
 */
void Client::onServiceAdded(types::device::Device server) {
  // if Discover Configuration is null the return
  if (this->m_ssl_config.isNull()) {
    throw std::runtime_error("SSL Config Config is not set");
  }

  // if already found
  if (std::find(m_servers.begin(), m_servers.end(), server) != m_servers.end()) {
    return;
  }

  // emit server found
  emit OnServerFound(server);

  // add to list
  this->m_servers.append(server);

  // emit the signal
  emit OnServerListChanged(getServerList());
}

/**
 * @brief On server removed function that That Called by the
 * discovery client when the server is removed
 */
void Client::onServiceRemoved(types::device::Device server) {
  // matcher to get the Device from servers list
  auto matcher = [server](const types::device::Device& device) {
    return device == server;
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
