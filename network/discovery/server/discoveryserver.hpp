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
#include "network/packets/discoverypacket/discoverypacket.hpp"
#include "network/packets/invalidrequest/invalidrequest.hpp"
#include "types/enums/enums.hpp"
#include "types/except/except.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::discovery {
/**
 * @brief Abstract Discovery server that Listens for the client that send
 * the broadcast message The user of this class should implement the
 * getIpType(), getIPAddress() and getPort() functions to return the
 * IP type, IP address and port number respectively
 */
class DiscoveryServer : public QObject {
 private:  // typedefs for this class

  using MalformedPacket = types::except::MalformedPacket;
  using IPType          = types::enums::IPType;

 private:  // Just for Qt

  Q_OBJECT

 private:  // variables

  QUdpSocket* m_socket = new QUdpSocket(this);

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DiscoveryServer)

 private:  // private functions

  /**
   * @brief Create the packet and send it to the client
   *
   * @param packet Packet to send
   * @param host Host address
   * @param port Port number
   */
  template <typename Packet>
  void sendPacket(const Packet& pack, const QHostAddress& host, quint16 port) {
    m_socket->writeDatagram(utility::functions::toQByteArray(pack), host, port);
  }

  /**
   * @brief Process the packet and return the packet
   * with server Information
   */
  void processDiscoveryPacket(const packets::DiscoveryPacket& packet);

  /**
   * @brief Process the datagrams that are received
   * from the socket
   */
  void processDatagrams();

 public:  // public functions

  /**
   * @brief Construct a new Discovery Server object
   *
   * @param parent Parent object
   */
  explicit DiscoveryServer(QObject* parent = nullptr);

 protected:  // protected functions

  /**
   * @brief Destroy the Discovery Server object
   */
  virtual ~DiscoveryServer()                     = default;

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
  virtual quint16 getPort() const                = 0;

  /**
   * @brief Get the IP Address of the server it can be IPv4 or IPv6
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QHostAddress getIPAddress() const      = 0;

 public:

  /**
   * @brief Start the server
   */
  virtual void startServer();
  /**
   * @brief Stop the server
   */
  virtual void stopServer();
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
