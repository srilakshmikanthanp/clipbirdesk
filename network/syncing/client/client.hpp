#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>
#include <QSslSocket>
#include <QTimer>
#include <QVector>

// standard headers
#include <tuple>
#include <utility>

// Local headers
#include "network/service/discover/discover.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing client that syncs the clipboard data between
 * client and server
 */
class Client : public service::Discover {
 signals:  // signals for this class
  /// @brief On Server List Changed
  void OnServerListChanged(QList<QPair<QHostAddress, quint16>> servers);

 signals:  // signals for this class
  /// @brief On Server Found
  void OnServerFound(QPair<QHostAddress, quint16>);

 signals:  // signals for this class
  /// @brief On Server Gone
  void OnServerGone(QPair<QHostAddress, quint16>);

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 signals:  // signals for this class
  /// @brief On Server state changed
  void OnServerStatusChanged(bool isConnected);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  /// @brief Disable copy and move
  Q_DISABLE_COPY_MOVE(Client)

 private:  // Member variables

  /// @brief List of Found servers and timestamp
  QList<std::tuple<QHostAddress, quint16>> m_servers;

  /// @brief SSL socket
  QSslSocket* m_ssl_socket = new QSslSocket(this);

  /// @brief Timer to update the server list
  QTimer* m_timer          = new QTimer(this);

  /// @brief Threshold times
  const qint64 m_threshold = 10000;

 private:  // private functions

  /**
   * @brief Create the packet and send it to the client
   *
   * @param packet Packet to send
   */
  template <typename Packet>
  void sendPacket(const Packet& pack) {
    // Convert the packet to QByteArray
    const auto data = utility::functions::toQByteArray(pack);

    // create the QDataStream
    QDataStream stream(m_ssl_socket);

    // write the data to the stream
    auto wrote = 0;

    // write the packet length
    while (wrote < data.size()) {
      // try to write the data
      auto start = data.data() + wrote;
      auto size  = data.size() - wrote;
      auto bytes = stream.writeRawData(start, size);

      // if error occurred
      if (bytes == -1) {
        emit OnErrorOccurred(m_ssl_socket->errorString());
        return;
      }

      // update the wrote
      wrote += bytes;
    }
  }

  /**
   * @brief Process the packet that has been received
   * from the server and emit the signal
   *
   * @param packet Syncing packet
   */
  void processSyncingPacket(const packets::SyncingPacket& packet);

  /**
   * @brief Process the Invalid packet that has been received
   * from the server and emit the signal
   *
   * @param packet Invalid packet
   */
  void processInvalidPacket(const packets::InvalidRequest& packet);

  /**
   * @brief Process the packet that has been received
   * from the server
   */
  void processReadyRead();

  /**
   * @brief Process the ssl error
   */
  void processSslError(const QList<QSslError>& errors);

  /**
   * @brief Handle client connected
   */
  void handleConnected();

  /**
   * @brief Handle client disconnected
   */
  void handleDisconnected();

 public:

  /**
   * @brief Construct a new Syncing Client object
   * and connect the signals and slots and start
   * the timer and Service discovery
   *
   * @param th threshold
   * @param parent Parent
   */
  Client(QObject* parent = nullptr);

  /**
   * @brief Destroy the Syncing Client object
   */
  ~Client() override = default;

  /**
   * @brief Send the items to the server to sync the
   * clipboard data
   *
   * @param items QVector<QPair<QString, QByteArray>>
   */
  void syncItems(QVector<QPair<QString, QByteArray>> items);

  /**
   * @brief Get the Server List object
   *
   * @return QList<QPair<QHostAddress, quint16>> List of servers
   */
  QList<QPair<QHostAddress, quint16>> getServerList() const;

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(QPair<QHostAddress, quint16> client);

  /**
   * @brief Get the Connection Host and Port object
   * @return QPair<QHostAddress, quint16>
   */
  QPair<QHostAddress, quint16> getConnectedServer() const;

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer();

  /**
   * @brief Get the Connection Host and Port object Or Empty
   * @return QPair<QHostAddress, quint16>
   */
  QPair<QHostAddress, quint16> getConnectedServerOrEmpty() const;

 protected:  // abstract functions from the base class

  /**
   * @brief On server found function that That Called by the
   * discovery client when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  void onServerAdded(QPair<QHostAddress, quint16> server) override;

  /**
   * @brief On server removed function that That Called by the
   * discovery client when the server is removed
   *
   * @param server
   */
  void onServerRemoved(QPair<QHostAddress, quint16> server) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
