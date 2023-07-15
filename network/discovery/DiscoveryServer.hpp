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
#include "network/packets/InvalidRequest.hpp"
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
 private:   // typedefs for this class
 using MalformedPacket = types::except::MalformedPacket;
 using IPType = types::enums::IPType;

 signals:   // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 private:   // variables
  QUdpSocket m_socket = QUdpSocket(this);

 private: // disable copy and move
  Q_DISABLE_COPY_MOVE(DiscoveryServer)

 private:   // private functions
  /**
   * @brief Create the packet and send it to the client
   *
   * @param packet Packet to send
   * @param host Host address
   * @param port Port number
   */
  template <typename Packet>
  void sendPacket(const Packet& pack, const QHostAddress& host, quint16 port) {
    m_socket.writeDatagram(utility::functions::toQByteArray(pack), host, port);
  }

  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processDiscoveryPacket(const packets::DiscoveryPacket& packet) {
    // response type of the packet is response
    const auto pakType = packets::DiscoveryPacket::PacketType::Response;

    // Using the functions namespace
    using utility::functions::toIPV4QHostAddress;
    using utility::functions::createPacket;
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
      #undef  PARAMS // just used to avoid long line
    } catch (...) {
      return; // return if any error occurs
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
      QHostAddress addr; quint16 port;
      m_socket.readDatagram(data.data(), data.size(), &addr, &port);

      // Using the ipconv namespace to convert the IP address
      using utility::functions::fromQByteArray;
      using utility::functions::createPacket;

      // try to parse the packet if it
      // fails then continue to next
      try {
        this->processDiscoveryPacket(fromQByteArray<packets::DiscoveryPacket>(data));
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

 public:  // public functions
  /**
   * @brief Construct a new Discovery Server object
   *
   * @param parent Parent object
   */
  explicit DiscoveryServer(QObject* parent = nullptr): QObject(parent) {
    // Connect the socket to the callback function that
    // process the datagrams when the socket is ready
    // to read so the listener can be notified
    const auto signal = &QUdpSocket::readyRead;
    const auto slot = &DiscoveryServer::processDatagrams;
    QObject::connect(&m_socket, signal, this, slot);
  }

 protected:  // protected functions

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

public:

  /**
   * @brief Start the server
   */
  virtual void start() {
    const auto mode = QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint;
    const auto port = 0;
    const auto host = QHostAddress::AnyIPv4;
    m_socket.bind(host, port, mode);
  }

  /**
   * @brief Stop the server
   */
  virtual void stop() {
    m_socket.close();
  }
};
} // namespace srilakshmikanthanp::clipbirdesk::network::discovery
