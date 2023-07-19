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
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::service {
/**
 * @brief Discovery client that sends the broadcast message
 * to the server and listen for the response if any server
 * is found then the callback function is called
 */
class Discover : public QObject {
 private:  // private members variables

  /// @brief Udp socket
  QUdpSocket* m_socket = new QUdpSocket(this);

  /// @brief Timer to send the broadcast message
  QTimer* m_timer      = new QTimer(this);

 private:  // Just for Qt

  Q_OBJECT

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Discover)

 private:  // private functions

 public:

  /**
   * @brief Construct a new Discovery Discover object
   *
   * @param parent Parent object
   */
  explicit Discover(QObject* parent = nullptr);

  /**
   * @brief Destroy the Discovery Discover object
   */
  ~Discover();

  /**
   * @brief Starts the discovery client by sending the
   * broadcast message
   *
   * @param interval Interval between each broadcast
   */
  void startDiscovery();

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
  virtual void onServerAdded(QPair<QHostAddress, quint16>) = 0;

  /**
   * @brief On Server Removed abstract function that
   * is called when the server is removed
   *
   * @param host Host address
   * @param port Port number
   */
  virtual void onServerRemoved(QPair<QHostAddress, quint16>) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::discovery
