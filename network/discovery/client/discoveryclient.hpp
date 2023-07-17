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
 * @brief Discovery client that sends the broadcast message
 * to the server and listen for the response if any server
 * is found then the callback function is called
 */
class DiscoveryClient : public QObject {
 private:  // private members variables
  /// @brief Udp socket
  QUdpSocket m_socket = QUdpSocket(this);

  /// @brief Timer to send the broadcast message
  QTimer m_timer = QTimer(this);

 private:  // Just for Qt
  Q_OBJECT

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(DiscoveryClient)

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
    m_socket.writeDatagram(utility::functions::toQByteArray(pack), host, port);
  }

  /**
   * @brief Process the invalid packet
   */
  void processInvalidPacket(const packets::InvalidRequest& packet);

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

  /**
   * @brief Send the broadcast message to the local
   * network to find the server
   */
  void sendBroadcastMessage();

 public:
  /**
   * @brief Construct a new Discovery Client object
   *
   * @param parent Parent object
   */
  explicit DiscoveryClient(QObject* parent = nullptr);

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
  void startDiscovery(quint32 interval = 1000);

  /**
   * @brief Stops the discovery client
   */
  void stopDiscovery();

 protected:  // abstract functions
  /**
   * @brief On server found abstract function that
   * is called when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServerFound(QPair<QHostAddress, quint16>) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
