// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "client.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {

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
  emit OnErrorOccurred(packet.getErrorMessage());
}

/**
 * @brief Updates the server list by removing the
 * server that that has exceeded the threshold
 * and emit the signal
 */
void Client::updateServerList() {
  // current timestamp in milliseconds
  const auto current   = QDateTime::currentMSecsSinceEpoch();

  // candidate for removal
  const auto candidate = [&](auto element) -> bool {
    return (current - std::get<2>(element)) > m_threshold;
  };

  // remove the server that has exceeded the threshold
  const auto start = m_servers.begin();
  const auto end   = m_servers.end();
  const auto it    = std::remove_if(start, end, candidate);

  // if No change return
  if (it == end) return;

  // Remove the server from the list
  m_servers.erase(it, end);

  // emit the signal if changed
  emit OnServerListChanged(getServerList());
}

/**
 * @brief Process the packet that has been received
 * from the server
 */
void Client::processReadyRead() {
  // Read All the data from the socket
  const auto data = m_ssl_socket->readAll();

  // using fromQByteArray to parse the packet
  using utility::functions::fromQByteArray;

  // try to parse the packet
  try {
    processSyncingPacket(fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    OnErrorOccurred(e.what());
    return;
  } catch (const std::exception& e) {
    OnErrorOccurred(e.what());
    return;
  } catch (...) {
    OnErrorOccurred("Unknown Error");
    return;
  }

  // try to parse the packet
  try {
    processInvalidPacket(fromQByteArray<packets::InvalidRequest>(data));
    return;
  } catch (const types::except::MalformedPacket& e) {
    OnErrorOccurred(e.what());
    return;
  } catch (const std::exception& e) {
    OnErrorOccurred(e.what());
    return;
  } catch (...) {
    OnErrorOccurred("Unknown Error");
    return;
  }

  // if no packet is found
  OnErrorOccurred("Unknown Packet Found");
}

/**
 * @brief Construct a new Syncing Client object
 * and connect the signals and slots and start
 * the timer and Service discovery
 *
 * @param th threshold
 * @param parent Parent
 */
Client::Client(QObject* parent) : discovery::Client(parent) {
  // connect the signal to emit the signal for
  // OnErrorOccurred from the base class
  const auto signal_e = &discovery::Client::OnErrorOccurred;
  const auto slot_e   = &Client::OnErrorOccurred;
  connect(this, signal_e, this, slot_e);

  // connected signal to emit the signal for
  // server state changed
  const auto signal_c = &QSslSocket::connected;
  const auto slot_c   = [&]() { emit OnServerStatusChanged(true); };
  connect(m_ssl_socket, signal_c, this, slot_c);

  // connect the signals and slots for the socket
  // readyRead signal to process the packet
  const auto signal_r = &QSslSocket::readyRead;
  const auto slot_r   = &Client::processReadyRead;
  connect(m_ssl_socket, signal_r, this, slot_r);

  // connect the signal to emit the signal for
  // timer to update the server list
  const auto signal_t = &QTimer::timeout;
  const auto slot_t   = &Client::updateServerList;
  connect(m_timer, signal_t, this, slot_t);

  // start the timer to update the server list
  m_timer->start(m_threshold);

  // disconnected signal to emit the signal for
  // server state changed
  const auto signal_d = &QSslSocket::disconnected;
  const auto slot_d   = [&]() { emit OnServerStatusChanged(false); };
  connect(m_ssl_socket, signal_d, this, slot_d);
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
  using utility::functions::createPacket;

  // create the packet
  packets::SyncingPacket packet =
      createPacket({packets::SyncingPacket::PacketType::SyncPacket, items});

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
  for (auto& [host, port, _] : m_servers) {
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
  return {m_ssl_socket->peerAddress(), m_ssl_socket->peerPort()};
}

/**
 * @brief Disconnect from the server
 */
void Client::disconnectFromServer() {
  m_ssl_socket->disconnectFromHost();
}

/**
 * @brief Set the SSL Configuration object
 *
 * @param config SSL Configuration
 */
void Client::setSSLConfiguration(QSslConfiguration config) {
  m_ssl_socket->setSslConfiguration(config);
}

/**
 * @brief Get the SSL Configuration object
 *
 * @return QSslConfiguration
 */
QSslConfiguration Client::getSSLConfiguration() const {
  return m_ssl_socket->sslConfiguration();
}

/**
 * @brief On server found function that That Called by the
 * discovery client when the server is found
 *
 * @param host Host address
 * @param port Port number
 */
void Client::onServerFound(QPair<QHostAddress, quint16> server) {
  // current timestamp in milliseconds
  const auto current = QDateTime::currentMSecsSinceEpoch();

  // emit the signal
  emit OnServerFound(server);

  // add the server to the list
  m_servers.append({server.first, server.second, current});

  // emit the signal
  emit OnServerListChanged(getServerList());
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
