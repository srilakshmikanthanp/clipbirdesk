// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "discoveryserver.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery {

/**
 * @brief Process the packet and return the packet
 * with server Information
 */
void DiscoveryServer::processDiscoveryPacket(
    const packets::DiscoveryPacket& packet) {
  // response type of the packet is response
  const auto pakType = packets::DiscoveryPacket::PacketType::Response;

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
  if (type == IPType::IPv4) {
    address = toIPV4QHostAddress(host);
  } else {
    address = toIPV6QHostAddress(host);
  }

  // set the IP address and port number
  try {
#define PARAMS pakType, getIPType(), getIPAddress(), getPort()
    this->sendPacket(createPacket({PARAMS}), address, port);
#undef PARAMS  // just used to avoid long line
  } catch (...) {
    return;  // return if any error occurs
  }
}

/**
 * @brief Process the datagrams that are received
 * from the socket
 */
void DiscoveryServer::processDatagrams() {
  while (m_socket.hasPendingDatagrams()) {
    // Read the data from the socket
    QByteArray data(m_socket.pendingDatagramSize(), Qt::Uninitialized);
    QHostAddress addr;
    quint16 port;
    m_socket.readDatagram(data.data(), data.size(), &addr, &port);

    // Using the ipconv namespace to convert the IP address
    using utility::functions::createPacket;
    using utility::functions::fromQByteArray;

    // try to parse the packet if it
    // fails then continue to next
    try {
      this->processDiscoveryPacket(
          fromQByteArray<packets::DiscoveryPacket>(data));
      continue;
    } catch (MalformedPacket& e) {
      const auto pakType = packets::InvalidRequest::PacketType::RequestFailed;
      sendPacket(createPacket({pakType, e.getCode(), e.what()}), addr, port);
      continue;
    } catch (std::exception& e) {
      emit OnErrorOccurred(e.what());
      continue;
    } catch (...) {
      emit OnErrorOccurred("Unknown error");
      continue;
    }
  }
}

/**
 * @brief Construct a new Discovery Server object
 *
 * @param parent Parent object
 */
DiscoveryServer::DiscoveryServer(QObject* parent): QObject(parent) {
  // Connect the socket to the callback function that
  // process the datagrams when the socket is ready
  // to read so the listener can be notified
  const auto signal = &QUdpSocket::readyRead;
  const auto slot = &DiscoveryServer::processDatagrams;
  QObject::connect(&m_socket, signal, this, slot);
}

/**
 * @brief Start the server
 */
void DiscoveryServer::startServer() {
  const auto mode = QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint;
  const auto port = 0;
  const auto host = QHostAddress::AnyIPv4;
  m_socket.bind(host, port, mode);
}

/**
 * @brief Stop the server
 */
void DiscoveryServer::stopServer() { m_socket.close(); }
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
