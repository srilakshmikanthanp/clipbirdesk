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
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QtLogging>

// Local headers
#include "../../../network/packets/packets.hpp"
#include "../../../types/enums/enums.hpp"
#include "../../../types/except/except.hpp"
#include "../../../utility/functions/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class DiscoveryServer : public QObject {
 private:               // typedefs
  using InvalidPacket = types::except::InvalidPacket;  // Invalid packet exception

 private:               // variables
  QUdpSocket m_socket;  // Udp socket used to listen for the broadcast message

 private:  // functions
  /**
   * @brief Send the malformed packet to the client
   *
   * @param e Exception to send
   * @param host Host address
   * @param port Port number
   */
  void sendMalformedPacket(const InvalidPacket& e, const QHostAddress& host, quint16 port) {
    // using the ResponseType from the MalformedPacket
    const auto pakType = packets::MalformedPacket::PacketType::InvalidPacket;

    // Create the malformed packet to send
    packets::MalformedPacket malformedPacket;

    // set the packet type & length
    malformedPacket.setPacketType(pakType);

    // set the error code and msg
    malformedPacket.setErrorCode(e.getCode());
    malformedPacket.setErrorMessage(e.what());

    // set the Length of the packet
    malformedPacket.setPacketLength(malformedPacket.size());

    // Create the datagram and send it
    const auto datagram = malformedPacket.toNetBytes();

    // send the datagram
    m_socket.writeDatagram(datagram, host, port);
  }

  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processPacket(const packets::ServiceDiscoveryPacket& packet) {
    // Using the ipconv namespace to convert the IP address
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions::ipconv;

    // response type of the packet
    const auto pakType = packets::ServiceDiscoveryPacket::PacketType::Response;

    // get this client IP address & port number
    const auto host = packet.getHostIp();
    const auto port = packet.getHostPort();
    const auto type = packet.getIpType();

    // Create the QHostAddress from the IP address
    QHostAddress address;

    // convert the IP address to QHostAddress
    if (type == types::enums::IPType::IPv4) {
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
    sendPacket.setPacketLength(sendPacket.size());

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
      QHostAddress address;
      quint16 port;
      m_socket.readDatagram(data.data(), data.size(), &address, &port);

      // Convert the data to the packet
      packets::ServiceDiscoveryPacket packet;
      QDataStream stream(data);

      // try to parse the packet if it
      // fails then continue to next
      try {
        stream >> packet;
      } catch (types::except::InvalidPacket& e) {
        sendMalformedPacket(e, address, port);
        continue;
      } catch (...) {
        continue;
      }

      // Process the packet
      this->processPacket(packet);
    }
  }

 public:  // functions
  /**
   * @brief Construct a new Discovery Server object and bind
   * the socket to listen for the broadcast message
   *
   * @param parent Parent object
   */
  explicit DiscoveryServer(QObject* parent = nullptr)
      : QObject(parent), m_socket(this) {
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
    const auto slot = &DiscoveryServer::processDatagrams;
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
  virtual types::enums::IPType getIPType() const = 0;

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

/**
 * @brief Discovery client that sends the broadcast message
 * to the server and listen for the response if any server
 * is found then the callback function is called
 */
class DiscoveryClient : public QObject {
 private:
  /// @brief Udp socket
  QUdpSocket m_socket;  // Udp socket used to send the broadcast message

  /// @brief Timer to send the broadcast message
  QTimer m_timer;

 public:
  /// @brief OnServer found callback
  using OnServerFound = void (*)(const QHostAddress, const quint16);

 private:
  /// @brief listeners for the server found
  std::vector<OnServerFound> m_listeners;

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
    if (type == types::enums::IPType::IPv4) {
      address = toQHostIPv4Address(host);
    } else {
      address = toQHostIPv6Address(host);
    }

    // Notify the listeners
    for (const auto& listener : m_listeners) {
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
      QHostAddress address;
      quint16 port;

      // Read the datagram
      m_socket.readDatagram(data.data(), data.size(), &address, &port);

      // Check if the datagram is MalformedPacket
      packets::MalformedPacket malformedPacket;
      QDataStream m_stream(data);

      // try to parse the packet if it
      // fails then continue to next
      try {
        m_stream >> malformedPacket;
      } catch (types::except::InvalidPacket& e) {
        qInfo(e.what());
      } catch (...) {
        qInfo("Unknown Error");
      }

      // Convert the data to the packet
      packets::ServiceDiscoveryPacket packet;
      QDataStream p_stream(data);

      // try to parse the packet if it
      // fails then continue to next
      try {
        p_stream >> packet;
      } catch (types::except::InvalidPacket& e) {
        qErrnoWarning(e.what());
        continue;
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
    packet.setIpType(types::enums::IPType::IPv4);

    // set the length of the packet
    packet.setPacketLength(packet.size());

    // Send the data
    m_socket.writeDatagram(packet.toNetBytes(), QHostAddress::Broadcast, 0);
  }

 public:
  /**
   * @brief Construct a new Discovery Client object
   *
   * @param parent Parent object
   */
  explicit DiscoveryClient(QObject* parent = nullptr)
      : QObject(parent), m_timer(this) {
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
    const auto slot_u = &DiscoveryClient::processDatagrams;
    QObject::connect(&m_socket, signal_u, this, slot_u);

    // Connect the timer to the callback function that
    // sends the broadcast message
    const auto signal_t = &QTimer::timeout;
    const auto slot_t = &DiscoveryClient::sendBroadcastMessage;
    QObject::connect(&m_timer, signal_t, this, slot_t);
  }

  /**
   * @brief Add the listener for the server found
   * event.
   *
   * @param listener Listener to be added
   */
  void addOnServerFound(const OnServerFound& listener) {
    m_listeners.push_back(listener);
  }

  /**
   * @brief Remove the listener for the server found
   * event.
   *
   * @param listener Listener to be removed
   */
  void removeOnServerFound(const OnServerFound& listener) {
    m_listeners.erase(
        std::find(m_listeners.begin(), m_listeners.end(), listener));
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
  void stopDiscovery() { m_timer.stop(); }
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
