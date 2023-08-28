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
#include <QNetworkReply>

// standard headers
#include <tuple>
#include <utility>

// Local headers
#include "network/service/index.hpp"
#include "types/enums/enums.hpp"
#include "types/device/device.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing client that syncs the clipboard data between
 * client and server
 */
class Client : public service::mdnsBrowser {
 signals:  // signals for this class
  /// @brief On Server List Changed
  void OnServerListChanged(QList<types::device::Device> servers);

 signals:  // signals for this class
  /// @brief On Server Found
  void OnServerFound(types::device::Device);

 signals:  // signals for this class
  /// @brief On Server Gone
  void OnServerGone(types::device::Device);

 signals:  // signals for this class
  /// @brief On Server state changed
  void OnServerStatusChanged(bool isConnected);

 signals:  // signals for this class
  /// @brief On Server Auth Succeed
  void OnServerAuthentication(bool isSuccessful);

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
  QList<types::device::Device> m_servers;

  /// @brief SSL socket
  QSslSocket* m_ssl_socket = new QSslSocket(this);

  /// @brief is Server Authenticated Me
  bool m_is_authed = false;

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

    // start the transaction
    stream.startTransaction();

    // write the data to the stream
    auto wrote = 0;

    // write the packet length
    while (wrote < data.size()) {
      // try to write the data
      auto bytes = stream.writeRawData(data.data() + wrote, data.size() - wrote);

      // if no error occurred
      if (bytes != -1) {
        wrote += bytes; continue;
      }

      // abort the transaction
      stream.abortTransaction();

      // log
      qWarning() << LOG(m_ssl_socket->errorString().toStdString());
    }

    // commit the transaction
    stream.commitTransaction();
  }

  /**
   * @brief Process the Authentication Packet from the server
   *
   * @param packet Authentication
   */
  void processAuthentication(const packets::Authentication& packet);

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
   * @brief Set SSL configuration
   */
  void setSslConfiguration(const QSslConfiguration& config);

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
  QList<types::device::Device> getServerList() const;

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(types::device::Device client);

  /**
   * @brief IS connected to the server
   */
  bool isConnected() const;

  /**
   * @brief Get the Connection Host and Port object
   */
  types::device::Device getConnectedServer() const;

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer();

  /**
   * @brief Is client is authenticated
   */
  bool isAuthenticated() const;

  /**
   * @brief Get the Authed Server object
   */
  types::device::Device getAuthedServer() const;

  /**
   * @brief Send To Connected Server
   *
   * @tparam Packet
   * @param pack
   */
  template <typename Packet>
  void sendToConnectedServer(const Packet& pack) {
    // if not connected
    if (m_ssl_socket->state() != QAbstractSocket::ConnectedState) {
      throw std::runtime_error("Not connected to the server");
    }

    // send the packet
    sendPacket(pack);
  }

 protected:  // abstract functions from the base class

  /**
   * @brief On server found function that That Called by the
   * discovery client when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  void onServiceAdded(QPair<QHostAddress, quint16> server) override;

  /**
   * @brief On server removed function that That Called by the
   * discovery client when the server is removed
   *
   * @param server
   */
  void onServiceRemoved(QPair<QHostAddress, quint16> server) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
