#include "client.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::lan {
/**
 * @brief Verify Server
 */
bool Client::verifyCert(const QSslCertificate& certificate) {
  // common name of the server
  auto name = certificate.subjectInfo(QSslCertificate::CommonName).constFirst();

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
  ignoredErrors.append(QSslError::UnspecifiedError);
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
    qWarning() << std::to_string(error.error()) + " : " + error.errorString().toStdString();
  }

  // if errorsCopy is not empty
  if (!errorsCopy.isEmpty()) {
    return this->m_ssl_socket->abort();
  }

  // get certificate
  const auto certificate = this->m_ssl_socket->peerCertificate();

  // if any error occurred
  if (!this->verifyCert(certificate)) {
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
  ignoredErrors.append(QSslError::UnspecifiedError);
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
    qWarning() << std::to_string(error.error()).c_str();
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
  if (packet.getAuthStatus() != types::enums::AuthStatus::AuthOkay) {
    return;
  }

  // get the details of the server
  auto addr = m_ssl_socket->peerAddress();
  auto port = m_ssl_socket->peerPort();
  auto cert = m_ssl_socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();
  auto host = types::Device({addr, port, name});

  // emit the signal
  emit OnServerStatusChanged(true, host);

  // start the timer
  this->m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  this->m_pongTimer->start(constants::getAppMaxReadIdleTime());
}

/**
 * @brief Process the Invalid packet that has been received
 * from the server and emit the signal
 *
 * @param packet Invalid packet
 */
void Client::processInvalidPacket(const packets::InvalidRequest& packet) {
  emit OnInvalidPacket(packet.getErrorCode() , packet.getErrorMessage());
}

/**
 * @brief Precess the PingPongPacket from the client
 *
 * @param packet PingPongPacket
 */
void Client::processPingPacket(const packets::PingPongPacket &packet) {
  // using PingPongPacket Params
  using utility::functions::params::PingPacketParams;

  // using Ping Packet
  using packets::PingPongPacket;

  // if it is pong then ignore
  if (packet.getPingType() == types::enums::PingType::Pong) {
    return;
  }

  // create the PingPongPacket
  auto pingPacket = utility::functions::createPacket(PingPacketParams{
    PingPongPacket::PacketType::PingPong,
    types::enums::PingType::Pong
  });

  // send packet to the server
  this->sendPacket(pingPacket);

  // flush the socket
  this->m_ssl_socket->flush();
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
 * @brief Process Disconnection
 */
void Client::processDisconnection() {
  // get the peer server details
  auto addr = m_ssl_socket->peerAddress();
  auto port = m_ssl_socket->peerPort();
  auto cert = m_ssl_socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();
  auto host = types::Device({addr, port, name});

  // emit the signal
  emit OnServerStatusChanged(false, host);

  // stop the timer
  this->m_pingTimer->stop();

  // stop the timer
  this->m_pongTimer->stop();
}

/**
 * @brief Process the packet that has been received
 * from the server
 */
void Client::processReadyRead() {
  // using fromQByteArray to parse the packet
  using utility::functions::fromQByteArray;

  // set the last read time property
  m_ssl_socket->setProperty(READ_TIME, QDateTime::currentDateTime());

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

  // check has enough bytes
  if (m_ssl_socket->bytesAvailable() < toRead) {
    return get.rollbackTransaction();
  }

  // resize the data
  data.resize(packetLength);

  // read the data from the socket
  auto start = data.data() + data.size() - toRead;
  auto bytes = get.readRawData(start, toRead);

  // check for error
  if (bytes == -1) {
    return get.rollbackTransaction();
  }

  // update the toRead
  toRead -= bytes;

  // if toRead is not zero then return
  if (toRead) {
    return get.rollbackTransaction();
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
    qDebug() << e.what();
    return;
  } catch (const types::except::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // try to parse the packet
  try {
    processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const types::except::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // try to parse the packet
  try {
    processPingPacket(fromQByteArray<packets::PingPongPacket>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const types::except::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // try to parse the packet
  try {
    processInvalidPacket(fromQByteArray<packets::InvalidRequest>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const types::except::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // if no packet is found
  qDebug() << "Unknown Packet Found";
}

/**
 * @brief function to process the timeout
 */
void Client::processPingTimeout() {
  // using PingPongPacket Params
  using utility::functions::params::PingPacketParams;

  // using Ping Packet
  using packets::PingPongPacket;

  // create packet
  using utility::functions::createPacket;

  // create the PingPongPacket
  auto pingPacket = createPacket(PingPacketParams{
    PingPongPacket::PacketType::PingPong,
    types::enums::PingType::Ping
  });

  // send the packet to all the clients
  this->sendPacket(pingPacket);
}

/**
 * @brief function to process the timeout
 */
void Client::processPongTimeout() {
  auto lastRead = m_ssl_socket->property(READ_TIME).toDateTime();
  auto now      = QDateTime::currentDateTime();
  auto diff     = lastRead.msecsTo(now);

  if (diff > constants::getAppMaxReadIdleTime()) {
    m_ssl_socket->disconnectFromHost();
  }
}

/**
 * @brief Construct a new Syncing Client object
 * and connect the signals and slots and start
 * the timer and Service discovery
 *
 * @param th threshold
 * @param parent Parent
 */
Client::Client(QString serviceName, QString serviceType, QObject* parent) : Synchronizer(parent), m_mdnsBrowser(new mdns::MdnsBrowser(serviceName, serviceType, this)) {
  // connect the signals and slots for the errorOccurred
  connect(
    m_ssl_socket, &QSslSocket::errorOccurred,
    [=, this]{
      this->OnConnectionError(this->m_ssl_socket->errorString());
    }
  );

  // connect signals for the mDNS Browser
  connect(
    m_mdnsBrowser, &mdns::MdnsBrowser::onServiceAdded,
    this, &Client::handleServiceAdded
  );

  connect(
    m_mdnsBrowser, &mdns::MdnsBrowser::onServiceRemoved,
    this, &Client::handleServiceRemoved
  );

  // connect the signals and slots for the socket
  // readyRead signal to process the packet
  connect(
    m_ssl_socket, &QSslSocket::readyRead,
    this, &Client::processReadyRead
  );

  // disconnected signal to emit the signal for
  // server state changed
  connect(
    m_ssl_socket, &QSslSocket::disconnected,
    this, &Client::processDisconnection
  );

  // Connect the socket to the callback function that
  // process the timeout
  QObject::connect(
    m_pingTimer, &QTimer::timeout,
    this, &Client::processPingTimeout
  );

  // Connect the socket to the callback function that
  // process the timeout
  QObject::connect(
    m_pongTimer, &QTimer::timeout,
    this, &Client::processPongTimeout
  );
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
void Client::synchronize(const QVector<QPair<QString, QByteArray>>& items) {
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
QList<types::Device> Client::getServerList() const {
  return m_servers;
}

/**
 * @brief Connect to the server with the given host and port
 * number
 *
 * @param host Host address
 * @param port Port number
 */
void Client::connectToServer(types::Device server) {
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
  qInfo() << "Connecting to server: " + host.toStdString() + ":" + std::to_string(port);

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
}

/**
 * @brief Connect to server Secured
 */
void Client::connectToServerSecured(types::Device server) {
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
  qInfo() << "Connecting to server: " + host.toStdString() + ":" + std::to_string(port);

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
}

/**
 * @brief Get the Connection Host and Port object
 * @return QPair<QHostAddress, quint16>
 */
std::optional<types::Device> Client::getConnectedServer() const {
  // check if the socket is connected else throw error
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    return std::optional<types::Device>();
  }

  // peer server details
  auto addr = m_ssl_socket->peerAddress();
  auto port = m_ssl_socket->peerPort();
  auto cert = m_ssl_socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // return the host address and port number
  return std::optional<types::Device>({addr, port, name});
}

/**
 * @brief Check if the client is connected to the server
 */
bool Client::isConnectedToServer() const {
  return this->m_ssl_socket->state() == QAbstractSocket::ConnectedState;
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
void Client::handleServiceAdded(types::Device server) {
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
  emit OnServerListChanged(this->getConnectedServer(), getServerList());
}

/**
 * @brief On server removed function that That Called by the
 * discovery client when the server is removed
 */
void Client::handleServiceRemoved(types::Device server) {
  // matcher to get the Device from servers list
  auto matcher = [server](const types::Device& device) {
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

  // if connected to the server
  if (this->getConnectedServer() && *device == this->getConnectedServer().value()) {
    this->disconnectFromServer();
  }

  // emit server gone
  emit OnServerGone(*device);

  // remove the server from the list
  m_servers.erase(device);

  // emit the signal
  emit OnServerListChanged(this->getConnectedServer(), getServerList());
}

/**
 * @brief Get the browser object
 */
mdns::MdnsBrowser* Client::getMdnsBrowser() const {
  return m_mdnsBrowser;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
