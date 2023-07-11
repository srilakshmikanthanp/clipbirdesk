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
#include "network/discovery/DiscoveryClient.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv.hpp"
#include "utility/functions/nbytes.hpp"
#include "utility/functions/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing client that syncs the clipboard data between
 * client and server
 */
class SyncingClient : public discovery::DiscoveryClient {
 signals:  // signals for this class
  /// @brief On Server List Changed
  void OnServerListChanged(QList<QPair<QHostAddress, quint16>> servers);

 signals:  // signals for this class
  /// @brief On Server Found
  void OnServerFound(QHostAddress host, quint16 port);

 signals:  // signals for this class
  /// @brief On Server state changed
  void OnServerStateChanged(bool isConnected);

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 private:  // just for Qt
  /// @brief Qt meta object
  Q_OBJECT

 private:  // Member variables
  /// @brief List of Found servers and timestamp
  QList<std::tuple<QHostAddress, quint16, qint64>> m_servers;

  /// @brief SSL socket
  QSslSocket m_ssl_socket;

  /// @brief Timer to update the server list
  QTimer m_timer;

  /// @brief Threshold times
  qint64 m_threshold = 10000;

 private: // functions
   /**
   * @brief Process the packet that has been received
   * from the server and emit the signal
   *
   * @param packet Syncing packet
   */
  void processPacket(const packets::SyncingPacket& packet) {
    // Make the vector of QPair<QString, QByteArray>
    QVector<QPair<QString, QByteArray>> items;

    // Get the items from the packet
    for (auto item : packet.getItems()) {
      items.append({
        item.getMimeType().toStdString().c_str(), item.getPayload()
      });
    }

    // emit the signal
    emit OnSyncRequest(items);
  }

  /**
   * @brief Send the packet to the server to sync the
   * clipboard data
   *
   * @param items packet Syncing packet
   */
  void sendPacket(const packets::SyncingPacket& packet) {
    // using createPacket to create the packet
    using utility::functions::toQByteArray;
    // send the packet to the server
    m_ssl_socket.write(toQByteArray(packet));
  }

 private: // slots
  /**
   * @brief Updates the server list by removing the
   * server that that has exceeded the threshold
   * and emit the signal
   */
  void updateServerList() {
    // current timestamp in milliseconds
    const auto current = QDateTime::currentMSecsSinceEpoch();

    // candidate for removal
    const auto candidate = [&](auto element) -> bool {
      return (current - std::get<2>(element)) > m_threshold;
    };

    // remove the server that has exceeded the threshold
    const auto start = m_servers.begin();
    const auto end = m_servers.end();
    const auto it = std::remove_if(start, end, candidate);

    // if No change return
    if (it == end) return;

    // Remove the server from the list
    m_servers.erase(it, end);

    // emit the signal if changed
    emit OnServerListChanged(getServerList());
  }

  /**
   * @brief Process the packet that has been received
   * from the server
   */
  void processReadyRead() {
    // Read All the data from the socket
    const auto data = m_ssl_socket.readAll();

    // using fromQByteArray to parse the packet
    using utility::functions::fromQByteArray;

    // get the packet from the data
    packets::SyncingPacket syncPacket;

    // try to parse the packet
    try {
      syncPacket = fromQByteArray<packets::SyncingPacket>(data);
      processPacket(syncPacket);
      return;
    } catch (const types::except::MalformedPacket& e) {
      OnErrorOccurred(e.what());
      return;
    } catch (const std::exception& e) {
      OnErrorOccurred(e.what());
      return;
    } catch (...) {
      OnErrorOccurred("Unknown Error");
      return;
    }

    // get the packet from the data
    packets::InvalidPacket invalidPacket;

    // try to parse the packet
    try {
      invalidPacket = fromQByteArray<packets::InvalidPacket>(data);
      OnErrorOccurred(invalidPacket.getErrorMessage());
      return;
    } catch (const types::except::MalformedPacket& e) {
      OnErrorOccurred(e.what());
      return;
    } catch (const std::exception& e) {
      OnErrorOccurred(e.what());
      return;
    } catch (...) {
      OnErrorOccurred("Unknown Error");
      return;
    }

    // if no packet is found
    OnErrorOccurred("Unknown Packet Found");
  }

 protected:  // abstract functions
  /**
   * @brief On Error Occurred this function is called when any error
   * occurs in the socket
   *
   * @param error Error message
   */
  virtual void OnErrorOccurred(QString error) {
    emit OnErrorOccurred(error);
  }

  /**
   * @brief On server found function that That Called by the
   * discovery client when the server is found
   *
   * @param host Host address
   * @param port Port number
   */
  void onServerFound(const QHostAddress& host, quint16 port) {
    // current timestamp in milliseconds
    const auto current = QDateTime::currentMSecsSinceEpoch();

    // emit the signal
    emit OnServerFound(host, port);

    // add the server to the list
    m_servers.append({host, port, current});

    // emit the signal
    emit OnServerListChanged(getServerList());
  }

 public:
  /**
   * @brief Construct a new Syncing Client object
   * and connect the signals and slots and start
   * the timer and Service discovery
   *
   * @param rm Remove threshold
   * @param ec Search threshold
   * @param parent Parent
   */
  SyncingClient(quint64 th, QObject* parent = nullptr)
      : m_threshold(th), DiscoveryClient(parent) {
    // connected signal to emit the signal for
    // server state changed
    const auto signal_c = &QSslSocket::connected;
    const auto slot_c = [&]() { emit OnServerStateChanged(true); };
    connect(&m_ssl_socket, signal_c, this, slot_c);


    // connect the signals and slots for the socket
    // readyRead signal to process the packet
    const auto signal_r = &QSslSocket::readyRead;
    const auto slot_r = &SyncingClient::processReadyRead;
    connect(&m_ssl_socket, signal_r, this, slot_r);

    // connect the signal to emit the signal for
    // timer to update the server list
    const auto signal_t = &QTimer::timeout;
    const auto slot_t = &SyncingClient::updateServerList;
    connect(&m_timer, signal_t, this, slot_t);

    // disconnected signal to emit the signal for
    // server state changed
    const auto signal_d = &QSslSocket::disconnected;
    const auto slot_d = [&]() { emit OnServerStateChanged(false); };
    connect(&m_ssl_socket, signal_d, this, slot_d);
  }

  /**
   * @brief Destroy the Syncing Client object
   */
  ~SyncingClient() override = default;

  /**
   * @brief Get the Server List object
   *
   * @return QList<QPair<QHostAddress, quint16>> List of servers
   */
  QList<QPair<QHostAddress, quint16>> getServerList() {
    // Host address and port number
    QList<QPair<QHostAddress, quint16>> list;

    // iterate and add the server
    for (auto& [host, port, _] : m_servers) {
      list.append({host, port});
    }

    // return the list
    return list;
  }

  /**
   * @brief Send the items to the server to sync the
   * clipboard data
   *
   * @param items QVector<QPair<QString, QByteArray>>
   */
  void syncWithServer(QVector<QPair<QString, QByteArray>> items) {
    // check if the socket is connected else throw error
    if (!m_ssl_socket.isOpen()) {
      throw std::runtime_error("Socket is not connected");
    }

    // using createPacket to create the packet
    using utility::functions::createPacket;

    // create the packet
    packets::SyncingPacket packet = createPacket({
      packets::SyncingPacket::PacketType::SyncPacket, items
    });

    // send the packet to the server
    this->sendPacket(packet);
  }

  /**
   * @brief Connect to the server with the given host and port
   * number
   *
   * @param host Host address
   * @param port Port number
   */
  void connectToServer(const QHostAddress& host, quint16 port) {
    m_ssl_socket.connectToHostEncrypted(host.toString(), port);
  }

  /**
   * @brief Disconnect from the server
   */
  void disconnectFromServer() {
    m_ssl_socket.disconnectFromHost();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
