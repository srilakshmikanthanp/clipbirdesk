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
  if (packet.getAuthStatus() == types::enums::AuthStatus::AuthSuccess) {
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
  emit OnErrorOccurred(LOG(packet.getErrorMessage().toStdString()));
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
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    emit OnErrorOccurred(LOG(e.what()));
  } catch (const std::exception& e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (...) {
    emit OnErrorOccurred(LOG("Unknown Error"));
    return;
  }

  // try to parse the packet
  try {
    processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    emit OnErrorOccurred(LOG(e.what()));
  } catch (const std::exception& e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (...) {
    emit OnErrorOccurred(LOG("Unknown Error"));
    return;
  }

  // try to parse the packet
  try {
    processInvalidPacket(fromQByteArray<packets::InvalidRequest>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (const types::except::NotThisPacket& e) {
    emit OnErrorOccurred(LOG(e.what()));
  } catch (const std::exception& e) {
    emit OnErrorOccurred(LOG(e.what()));
    return;
  } catch (...) {
    emit OnErrorOccurred(LOG("Unknown Error"));
    return;
  }

  // if no packet is found
  OnErrorOccurred(LOG("Unknown Packet Found"));
}

/**
 * @brief Process the SSL error and emit the signal
 *
 * @param errors List of SSL errors
 */
void Client::processSslError(const QList<QSslError>& errors) {
  for (auto& error : errors) {
    emit OnErrorOccurred(LOG(error.errorString().toStdString()));
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
Client::Client(QObject* parent) : service::Discover(parent) {
  // connect the signal to emit the signal for
  // OnErrorOccurred from the base class
  const auto signal_e = &service::Discover::OnErrorOccurred;
  const auto slot_e   = &Client::OnErrorOccurred;
  connect(this, signal_e, this, slot_e);

  // connected signal to emit the signal for
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
QList<QPair<QHostAddress, quint16>> Client::getServerList() const {
  // Host address and port number
  QList<QPair<QHostAddress, quint16>> list;

  // iterate and add the server
  for (auto& [host, port] : m_servers) {
    list.append({host, port});
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
void Client::connectToServer(QPair<QHostAddress, quint16> client) {
  // check if the SSL configuration is set
  if (m_ssl_socket->sslConfiguration().isNull()) {
    throw std::runtime_error("SSL Configuration is not set");
  }

  // check if the socket is connected
  if (m_ssl_socket->isOpen()) {
    this->disconnectFromServer();
  }

  // create the host address
  const auto host = client.first.toString();
  const auto port = client.second;

  // connect to the server as encrypted
  m_ssl_socket->connectToHostEncrypted(host, port);
}

/**
 * @brief Get the Connection Host and Port object
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> Client::getConnectedServer() const {
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  return {m_ssl_socket->peerAddress(), m_ssl_socket->peerPort()};
}

/**
 * @brief Get the Connection Host and Port object Or Empty
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> Client::getConnectedServerOrEmpty() const {
  try {
    return this->getConnectedServer();
  } catch (const std::exception&) {
    return QPair<QHostAddress, quint16>();
  }
}

/**
 * @brief Disconnect from the server
 */
void Client::disconnectFromServer() {
  m_ssl_socket->disconnectFromHost();
}

/**
 * @brief Get the Authed Server object
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> Client::getAuthedServer() const {
  if (this->m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
    throw std::runtime_error("Socket is not connected");
  }

  if (!this->m_is_authed) {
    throw std::runtime_error("Socket is not authenticated");
  }

  return {m_ssl_socket->peerAddress(), m_ssl_socket->peerPort()};
}

/**
 * @brief Get the Authed Server object Or Empty
 * @return QPair<QHostAddress, quint16>
 */
QPair<QHostAddress, quint16> Client::getAuthedServerOrEmpty() const {
  try {
    return this->getAuthedServer();
  } catch (const std::exception&) {
    return QPair<QHostAddress, quint16>();
  }
}

/**
 * @brief On server found function that That Called by the
 * discovery client when the server is found
 *
 * @param host Host address
 * @param port Port number
 */
void Client::onServerAdded(QPair<QHostAddress, quint16> server) {
  // emit the signal for server found event
  emit OnServerFound(server);

  // add the server to the list
  m_servers.append({server.first, server.second});

  // emit the signal
  emit OnServerListChanged(getServerList());
}

/**
 * @brief
 *
 */
void Client::onServerRemoved(QPair<QHostAddress, quint16> server) {
  // emit the signal for server gone event
  emit OnServerGone(server);

  // remove the server from the list
  m_servers.removeAll({server.first, server.second});

  // emit the signal
  emit OnServerListChanged(getServerList());
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
