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
#include "network/packets/DiscoveryPacket.hpp"
#include "network/packets/InvalidPacket.hpp"
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"
#include "utility/functions/ipconv.hpp"
#include "utility/functions/nbytes.hpp"
#include "utility/functions/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class DiscoveryServer : public QObject {
 private:   // typedefs
 using MalformedPacket = types::except::MalformedPacket;
 using IPType = types::enums::IPType;

 private:   // variables
  QUdpSocket m_socket = QUdpSocket(this);

 private:  // functions
  /**
   * @brief Send the malformed packet to the client
   *
   * @param e Exception to send
   * @param host Host address
   * @param port Port number
   */
  void sendInvalidPacket(const MalformedPacket& e, const QHostAddress& host, quint16 port) {
    // using the ResponseType from the MalformedPacket
    const auto pakType = packets::InvalidPacket::PacketType::MalformedPacket;

    // using the functions namespace
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

    // Create the malformed packet to send
    packets::InvalidPacket invalidPacket = createPacket({
      pakType, e.getCode(), e.what()
    });

    // Create the datagram and send it
    const auto datagram = toQByteArray(invalidPacket);

    // send the datagram
    m_socket.writeDatagram(datagram, host, port);
  }

  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processPacket(const packets::DiscoveryPacket& packet) {
    // Using the ipconv namespace to convert the IP address
    using namespace srilakshmikanthanp::clipbirdesk::utility::functions;

    // response type of the packet
    const auto pakType = packets::DiscoveryPacket::PacketType::Response;

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

    // Create the packet to send
    packets::DiscoveryPacket sendPacket;

    // set the IP address and port number
    try {
      sendPacket = createPacket({
        pakType, getIPType(),getIPAddress(), getPort()
      });
    } catch (...) {
      return; // return if any error occurs
    }

    // Create the datagram and send it
    const auto datagram = toQByteArray(sendPacket);

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
      packets::DiscoveryPacket packet;
      QDataStream stream(data);

      // try to parse the packet if it
      // fails then continue to next
      try {
        stream >> packet;
      } catch (MalformedPacket& e) {
        sendInvalidPacket(e, address, port);
        continue;
      } catch (std::exception& e) {
        OnErrorOccurred(e.what());
        continue;
      } catch (...) {
        OnErrorOccurred("Unknown error");
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
  explicit DiscoveryServer(QObject* parent = nullptr): QObject(parent) {
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

 protected:  // functions

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
   * @throw Any Exception If any error occurs
   */
  virtual types::enums::IPType getIPType() const = 0;

  /**
   * @brief Get the Port number of the server it can be any port number
   * from 0 to 65535 but the port number should be greater than 1024
   * because the port number less than 1024 are reserved for the system
   * services
   *
   * @return types::Port Port number
   * @throw Any Exception If any error occurs
   */
  virtual quint16 getPort() const = 0;

  /**
   * @brief Get the IP Address of the server it can be IPv4 or IPv6
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QHostAddress getIPAddress() const = 0;

  /**
   * @brief On Error Occurred this function is called when any error
   * occurs in the socket
   *
   * @param error Error message
   */
  virtual void OnErrorOccurred(QString error) {
    qErrnoWarning(error.toStdString().c_str());
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::discovery
