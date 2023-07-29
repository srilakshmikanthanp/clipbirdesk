#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>
#include <QSslSocket>
#include <QVector>

#include "network/packets/authentication/authentication.hpp"
#include "network/service/register/register.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing server that syncs the clipboard data between
 * the clients
 */
class Server : public service::Register {
 signals:  // signals
  /// @brief On client state changed
  void OnCLientStateChanged(QPair<QHostAddress, quint16>, bool connected);

 signals:  // signals for this class
  /// @brief On New Host Connected
  void OnNewHostConnected(QPair<QHostAddress, quint16>);

 signals:  // signals for this class
  /// @brief On Error Occurred
  void OnErrorOccurred(QString error);

 signals:  // signals for this class
  /// @brief On Server State Changed
  void OnServerStateChanged(bool started);

 signals:  // signals
  /// @brief On Sync Request
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnClientListChanged(QList<QPair<QHostAddress, quint16>> clients);


 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Server)

 private:  // members of the class

  /// @brief SSL server
  QSslServer* m_ssl_server = new QSslServer(this);

  /// @brief List of clients unauthenticated
  QList<QSslSocket*> m_un_authed_clients;

  /// @brief List of clients Authenticated
  QList<QSslSocket*> m_authed_clients;

 private:  // some typedefs

  using MalformedPacket = types::except::MalformedPacket;

 private:  // member functions

  /**
   * @brief Create the packet and send it to the client
   *
   * @param client Client to send
   * @param packet Packet to send
   */
  template <typename Client, typename Packet>
  void sendPacket(Client* client, const Packet& pack) {
    // Convert the packet to QByteArray
    const auto data = utility::functions::toQByteArray(pack);

    // create the QDataStream
    QDataStream stream(client);

    // start the transaction
    stream.startTransaction();

    // write the data to the stream
    auto wrote = 0;

    // write the packet length
    while (wrote < data.size()) {
      // try to write the data
      auto start = data.data() + wrote;
      auto size  = data.size() - wrote;
      auto bytes = stream.writeRawData(start, size);

      // if no error occurred
      if (bytes != -1) {
        wrote += bytes; continue;
      }

      // abort the transaction
      stream.abortTransaction();

      //  Notifies the error occurred
      emit OnErrorOccurred(client->errorString());
    }

    // commit the transaction
    stream.commitTransaction();
  }

  /**
   * @brief Create the packet and send it to all the
   * clients
   *
   * @param packet Packet to send
   */
  template <typename Packet>
  void sendPacket(const Packet& pack) {
    for (auto client : m_authed_clients) {
      this->sendPacket(client, pack);
    }
  }

  /**
   * @brief Process the SyncingPacket from the client
   *
   * @param packet SyncingPacket
   */
  void processSyncingPacket(const packets::SyncingPacket& packet);

  /**
   * @brief Process the connections that are pending
   */
  void processConnections();

  /**
   * @brief Callback function that process the ready
   * read from the client
   */
  void processReadyRead();

  /**
   * @brief Process the disconnection from the client
   */
  void processDisconnection();

  /**
   * @brief On Service Registered
   */
  void OnServiceRegistered();

 public:  // constructors and destructors

  /**
   * @brief Construct a new Syncing Server object and
   * bind to any available port and any available
   * IP address
   *
   * @param config SSL configuration
   * @param parent Parent object
   */
  explicit Server(QObject* p = nullptr);

  /**
   * @brief Destroy the Syncing Server object
   */
  virtual ~Server() = default;

  /**
   * @brief Request the clients to sync the clipboard items
   *
   * @param data QVector<QPair<QString, QByteArray>>
   */
  void syncItems(QVector<QPair<QString, QByteArray>> items);

  /**
   * @brief Get the Clients that are connected to the server
   *
   * @return QList<QSslSocket*> List of clients
   */
  QList<QPair<QHostAddress, quint16>> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   *
   * @param client Client to disconnect
   */
  void disconnectClient(QPair<QHostAddress, quint16> client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief Get the Server QHostAddress & Port
   *
   * @return QPair<QHostAddress, quint16>
   */
  QPair<QHostAddress, quint16> getServerInfo() const;

  /**
   * @brief Set the SSL Configuration object
   *
   * @param config SSL Configuration
   */
  void setSSLConfiguration(QSslConfiguration config);
  /**
   * @brief Get the SSL Configuration object
   *
   * @return QSslConfiguration
   */
  QSslConfiguration getSSLConfiguration() const;

  /**
   * @brief The function that is called when the client is authenticated
   *
   * @param client the client that is currently processed
   */
  void authSuccess(const QPair<QHostAddress, quint16>&);

  /**
   * @brief The function that is called when the client it not
   * authenticated
   *
   * @param client the client that is currently processed
   */
  void authFailed(const QPair<QHostAddress, quint16>&);

  /**
   * @brief Start the server
   */
  void startServer();

  /**
   * @brief Stop the server
   */
  void stopServer();

 protected:  // override functions from the base class

  /**
   * @brief Get the IP Type of the Server it can be IPv4 or
   * IPv6 the IP type is used to determine the length of the IP address
   * if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @note The IP Type used in Server is IPv4
   *
   * @return types::IPType IP type
   * @throw Any Exception If any error occurs
   */
  virtual types::enums::IPType getIPType() const override;

  /**
   * @brief Get the Port number of the Server it can be any port
   * number from 0 to 65535 but the port number should be greater than 1024
   * because the port number less than 1024 are reserved for the system
   * services
   *
   * @return types::Port Port number
   * @throw Any Exception If any error occurs
   */
  virtual quint16 getPort() const override;

  /**
   * @brief Get the IP Address of the Server it can be IPv4 or
   * IPv6 if the IP type is IPv4 then the IP address is 4 bytes long if
   * the IP type is IPv6 then the IP address is 16 bytes long
   *
   * @return types::IPAddress IP address
   * @throw Any Exception If any error occurs
   */
  virtual QHostAddress getIPAddress() const override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
