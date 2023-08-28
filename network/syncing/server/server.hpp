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
#include "network/service/index.hpp"
#include "types/device/device.hpp"
#include "types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
/**
 * @brief Syncing server that syncs the clipboard data between
 * the clients
 */
class Server : public service::mdnsRegister {
 signals:  // signals
  /// @brief On client state changed
  void OnCLientStateChanged(types::device::Device, bool connected);

 signals:  // signals for this class
  /// @brief On New Host Connected
  void OnAuthRequested(types::device::Device);

 signals:  // signals for this class
  /// @brief On Server State Changed
  void OnServerStateChanged(bool started);

 signals:  // signals
  /// @brief On Sync Request
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 signals:  // signals for this class
  /// @brief On Sync Request
  void OnClientListChanged(QList<types::device::Device> clients);

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
  template <typename Packet>
  void sendPacket(QSslSocket* client, const Packet& pack) {
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
      auto bytes = stream.writeRawData(data.data() + wrote, data.size() - wrote);

      // if no error occurred
      if (bytes != -1) {
        wrote += bytes; continue;
      }

      // abort the transaction
      stream.abortTransaction();

      //  Notifies the error occurred
      qWarning() << (LOG(client->errorString().toStdString()));
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
  void sendPacket(const Packet& pack, QSslSocket* except = nullptr) {
    for (auto client : m_authed_clients) {
      if (client != except) sendPacket(client, pack);
    }
  }

  /**
   * @brief Process the AuthenticationPacket from the client
   *
   * @param packet
   */
  void processAuthenticationPacket(const packets::Authentication &packet);

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
   */
  QList<types::device::Device> getConnectedClientsList() const;

  /**
   * @brief Disconnect the client from the server and delete
   * the client
   *
   * @param client Client to disconnect
   */
  void disconnectClient(types::device::Device client);

  /**
   * @brief Disconnect the all the clients from the server
   */
  void disconnectAllClients();

  /**
   * @brief Get the Server QHostAddress & Port
   *
   * @return types::device::Device
   */
  types::device::Device getServerInfo() const;

  /**
   * @brief Set the SSL Configuration object
   *
   * @param config SSL Configuration
   */
  void setSslConfiguration(QSslConfiguration config);
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
  void authSuccess(const types::device::Device&);

  /**
   * @brief The function that is called when the client it not
   * authenticated
   *
   * @param client the client that is currently processed
   */
  void authFailed(const types::device::Device&);

  /**
   * @brief Start the server
   */
  void startServer();

  /**
   * @brief Stop the server
   */
  void stopServer();

  /**
   * @brief Send the packet to the client
   *
   * @param packet Packet to send
   * @param QPair {QHostAddress, port}
   */
  template <typename Packet>
  void sendToClient(const Packet &packet, const types::device::Device &client) {
    // Matcher Lambda Function to find the client
    const auto matcher = [&client](QSslSocket *c) {
      return (c->peerAddress() == client.ip) && (c->peerPort() == client.port);
    };

    // Get the iterator to the start and end of the list
    auto start      = m_authed_clients.begin();
    auto end        = m_authed_clients.end();

    // Get the client from the unauthenticated list and remove it
    auto client_itr = std::find_if(start, end, matcher);

    // If the client is not found then return from the function
    if (client_itr == m_authed_clients.end()) {
      throw std::runtime_error("Client not found");
    }

    // send the packet to the client
    this->sendPacket(packet, (*client_itr));
  }

 protected:  // override functions from the base class

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
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
