// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "discoveryclient.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery {
/**
 * @brief Process the invalid packet
 */
void DiscoveryClient::processInvalidPacket(const packets::InvalidRequest& packet) {
  emit OnErrorOccurred(packet.getErrorMessage());
}

/**
 * @brief Process the packet and return the packet
 * with server Information
 */
void DiscoveryClient::processDiscoveryPacket(const packets::DiscoveryPacket& packet) {
  // Using the functions namespace
  using utility::functions::createPacket;
  using utility::functions::toIPV4QHostAddress;
  using utility::functions::toIPV6QHostAddress;

  // get this client IP address & port number
  const auto host = packet.getHostIp();
  const auto port = packet.getHostPort();
  const auto type = packet.getIpType();

  // Create the QHostAddress from the IP address
  QHostAddress address;

  // convert the IP address to QHostAddress
  if (type == types::enums::IPType::IPv4) {
    this->onServerFound({toIPV4QHostAddress(host), port});
  } else {
    this->onServerFound({toIPV6QHostAddress(host), port});
  }
}

/**
 * @brief Process the datagrams that are received
 * from the socket
 */
void DiscoveryClient::processDatagrams() {
  while (m_socket->hasPendingDatagrams()) {
    // Read the data from the socket
    QByteArray data(m_socket->pendingDatagramSize(), Qt::Uninitialized);
    QHostAddress address;
    quint16 port;

    // Read the datagram
    m_socket->readDatagram(data.data(), data.size(), &address, &port);

    // using fromQByteArray to parse the packet
    using utility::functions::fromQByteArray;

    // try to parse the packet if it
    // fails then continue to next
    try {
      this->processDiscoveryPacket(fromQByteArray<packets::DiscoveryPacket>(data));
      continue;
    } catch (types::except::MalformedPacket& e) {
      emit OnErrorOccurred(e.what());
    } catch (std::exception& e) {
      emit OnErrorOccurred(e.what());
      continue;
    } catch (...) {
      emit OnErrorOccurred("Unknown Error");
      continue;
    }

    // try to parse the packet if it
    // fails then continue to next
    try {
      this->processInvalidPacket(fromQByteArray<packets::InvalidRequest>(data));
      continue;
    } catch (types::except::MalformedPacket& e) {
      emit OnErrorOccurred(e.what());
      continue;
    } catch (...) {
      emit OnErrorOccurred("Unknown Error");
      continue;
    }

    // log the error and continue
    emit OnErrorOccurred("Unknown Packet Found");
  }
}

/**
 * @brief Send the broadcast message to the local
 * network to find the server
 */
void DiscoveryClient::sendBroadcastMessage() {
  // make the necessary details to send the packet
  const auto pakT = packets::DiscoveryPacket::PacketType::Request;
  const auto host = m_socket->localAddress();
  const auto port = m_socket->localPort();

  // using the functions namespace
  using utility::functions::createPacket;

  // Create the packet
  packets::DiscoveryPacket packet = createPacket({pakT, types::enums::IPType::IPv4, host, port});

  // Send the data to the broadcast address
  this->sendPacket(packet, QHostAddress::Broadcast, 0);
}

/**
 * @brief Construct a new Discovery Client object
 *
 * @param parent Parent object
 */
DiscoveryClient::DiscoveryClient(QObject* parent) : QObject(parent) {
  // Connect the socket to the callback function that
  // process the datagrams when the socket is ready
  // to read so the listener can be notified
  const auto signal_u = &QUdpSocket::readyRead;
  const auto slot_u   = &DiscoveryClient::processDatagrams;
  QObject::connect(m_socket, signal_u, this, slot_u);

  // Bind the socket to listen for the broadcast message
  // The Host address is set to AnyIPv4 to listen for
  // the broadcast message and the port is set to 0 to
  // means that the OS will assign a port number
  const auto host = QHostAddress::AnyIPv4;
  const auto port = 0;
  m_socket->bind(host, port);

  // Connect the timer to the callback function that
  // sends the broadcast message
  const auto signal_t = &QTimer::timeout;
  const auto slot_t   = &DiscoveryClient::sendBroadcastMessage;
  QObject::connect(m_timer, signal_t, this, slot_t);
}

/**
 * @brief Starts the discovery client by sending the
 * broadcast message
 *
 * @param interval Interval between each broadcast
 */
void DiscoveryClient::startDiscovery(quint32 interval) {
  m_timer->start(interval);
}

/**
 * @brief Stops the discovery client
 */
void DiscoveryClient::stopDiscovery() {
  m_timer->stop();
}
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
