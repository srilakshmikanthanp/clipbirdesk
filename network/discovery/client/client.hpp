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
#include <QTimer>
#include <QUdpSocket>

// Local headers
#include "../../../network/packets/packets.hpp"
#include "../../../types/types.hpp"
#include "../../../utility/functions/ipconv.hpp"
#include "../../../utility/functions/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery::client {
/**
 * @brief Discovery client that sends the broadcast message
 * to the server and listen for the response if any server
 * is found then the callback function is called
 */
class DiscoveryClient : public QObject {
 public:
  /// @brief OnServer found callback
  using OnServerFound = std::function<void(const QHostAddress, const quint16)>;

 private:
  /// @brief listeners for the server found
  std::vector<OnServerFound> m_onServerFound;

  /// @brief Timer to send the broadcast message
  QTimer m_timer;

  /// @brief Udp socket
  QUdpSocket m_socket;  // Udp socket used to send the broadcast message

 private:  // functions
  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processPacket(const packets::ServiceDiscoveryPacket& packet) {
    // Using the ipconv namespace to convert the IP address
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::ipconv;

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

    // Notify the listeners
    for (const auto& listener : m_onServerFound) {
      listener(address, port);
    }
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

  /**
   * @brief Send the broadcast message
   */
  void sendBroadcastMessage() {
    // using the packet namespace deduce length of the packet
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::packet;

    // Using the ipconv namespace to convert the IP address
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::ipconv;

    // response type of the packet
    const auto pakType = packets::ServiceDiscoveryPacket::PacketType::Request;

    // Get the IP address and port number
    const auto host = toQByteArrayIPv4Address(m_socket.localAddress());
    const auto port = m_socket.localPort();

    // Create the packet
    packets::ServiceDiscoveryPacket packet;
    packet.setPacketType(pakType);
    packet.setHostIp(host);
    packet.setHostPort(port);
    packet.setIpType(types::IPType::IPv4);

    // set the length of the packet
    packet.setPacketLength(sizeOfPacket(packet));

    // Send the data
    m_socket.writeDatagram(packet.toNetBytes(), QHostAddress::Broadcast, 0);
  }

 public:
  /**
   * @brief Construct a new Discovery Client object
   *
   * @param parent Parent object
   */
  explicit DiscoveryClient(QObject* parent = nullptr) : QObject(parent), m_timer(this) {
    // Bind the socket to listen for the broadcast message
    // The Host address is set to AnyIPv4 to listen for
    // the broadcast message and the port is set to 0 to
    // means that the OS will assign a port number
    const auto host = QHostAddress::AnyIPv4;
    const auto port = 0;
    m_socket.bind(host, port);

    // Connect the socket to the callback function that
    // process the datagrams when the socket is ready
    // to read so the listener can be notified
    const auto signal_u = &QUdpSocket::readyRead;
    const auto slot_u   = &DiscoveryClient::processDatagrams;
    QObject::connect(&m_socket, signal_u, this, slot_u);

    // Connect the timer to the callback function that
    // sends the broadcast message
    const auto signal_t = &QTimer::timeout;
    const auto slot_t   = &DiscoveryClient::sendBroadcastMessage;
    QObject::connect(&m_timer, signal_t, this, slot_t);
  }

  /**
   * @brief Add the listener for the server found
   * event.
   *
   * @param listener Listener to be added
   */
  void addOnServerFound(const OnServerFound& listener) {
    m_onServerFound.push_back(listener);
  }

  /**
   * @brief Remove the listener for the server found
   * event.
   *
   * @param listener Listener to be removed
   */
  void removeOnServerFound(const OnServerFound& listener) {
    m_onServerFound.erase(std::find(m_onServerFound.begin(), m_onServerFound.end(), listener));
  }


  /**
   * @brief Destroy the Discovery Client object
   */
  ~DiscoveryClient() = default;

  /**
   * @brief Starts the discovery client by sending the
   * broadcast message
   *
   * @param interval Interval between each broadcast
   */
  void startDiscovery(const quint32 interval = 1000) {
    m_timer.start(interval);
  }

  /**
   * @brief Stops the discovery client
   */
  void stopDiscovery() {
    m_timer.stop();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery::client
