#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// C++ headers
#include <algorithm>
#include <vector>

// Qt headers
#include <QByteArray>
#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>

// Local headers
#include "../../../network/packets/packets.hpp"
#include "../../../types/types.hpp"
#include "../../../utility/functions/ipconv.hpp"
#include "../../../utility/functions/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery::server {

/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class DiscoveryServer : public QObject {
 private:  // variables
  QUdpSocket m_socket;  // Udp socket used to listen for the broadcast message

 private:  // functions
  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processPacket(const packets::ServiceDiscoveryPacket& packet) {
    // Using the ipconv namespace to convert the IP address
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::ipconv;

    // using the packet namespace deduce length of the packet
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::packet;

    // response type of the packet
    const auto pakType = packets::ServiceDiscoveryPacket::PacketType::Response;

    // get this client IP address & port number
    const auto host = packet.getHostIp();
    const auto port = packet.getHostPort();
    const auto type = packet.getIpType();

    // Create the QHostAddress from the IP address
    QHostAddress address;

    // convert the IP address to QHostAddress
    if (type == types::IPType::IPv4) {
      address = toQHostIPv4Address(host);
    } else {
      address = toQHostIPv6Address(host);
    }

    // Create the packet to send
    packets::ServiceDiscoveryPacket sendPacket;

    // set the IP address and port number
    sendPacket.setPacketType(pakType);
    sendPacket.setIpType(getIPType());
    sendPacket.setHostIp(getIPAddress());
    sendPacket.setHostPort(getPort());

    // set the Length of the packet
    sendPacket.setPacketLength(sizeOfPacket(sendPacket));

    // Create the datagram and send it
    const auto datagram = sendPacket.toNetBytes();

    // send the datagram
    m_socket.writeDatagram(datagram, address, port);
  }

  /**
   * @brief Process the datagrams that are received
   * from the socket
   */
  void processDatagrams() {
    while (m_socket.hasPendingDatagrams()) {
      // Read the data from the socket
      QByteArray data(m_socket.pendingDatagramSize(), Qt::Uninitialized);
      m_socket.readDatagram(data.data(), data.size());

      // Convert the data to the packet
      packets::ServiceDiscoveryPacket packet;
      QDataStream stream(data);

      // try to parse the packet if it
      // fails then continue to next
      try {
        stream >> packet;
      } catch (...) {
        continue;
      }

      // Process the packet
      this->processPacket(packet);
    }
  }

 public:   // functions
  /**
   * @brief Construct a new Discovery Server object and bind
   * the socket to listen for the broadcast message
   *
   * @param parent Parent object
   */
  explicit DiscoveryServer(QObject* parent = nullptr): QObject(parent), m_socket(this) {
    // Bind the socket to listen for the broadcast message
    // The Host address is set to AnyIPv4 to listen for
    // the broadcast message and the port is set to 0 to
    // means that the OS will assign a port number
    const auto mode = QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint;
    const auto port = 0;
    const auto host = QHostAddress::AnyIPv4;
    m_socket.bind(host, port, mode);

    // Connect the socket to the callback function that
    // process the datagrams when the socket is ready
    // to read so the listener can be notified
    const auto signal = &QUdpSocket::readyRead;
    const auto slot   = &DiscoveryServer::processDatagrams;
    QObject::connect(&m_socket, signal, this, slot);
  }

  /**
   * @brief Destroy the Discovery Server object
   */
  virtual ~DiscoveryServer() = default;

  /**
   * @brief Get the IP Type of the server it can be IPv4 or IPv6
   * the IP type is used to determine the length of the IP address
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPType IP type
   */
  virtual types::IPType getIPType() const = 0;

  /**
   * @brief Get the Port number of the server it can be any port number
   * from 0 to 65535 but the port number should be greater than 1024
   * because the port number less than 1024 are reserved for the system
   * services
   *
   * @return types::Port Port number
   */
  virtual quint16 getPort() const = 0;

  /**
   * @brief Get the IP Address of the server it can be IPv4 or IPv6
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   */
  virtual QByteArray getIPAddress() const = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery::server
