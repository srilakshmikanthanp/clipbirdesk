#pragma once

#include <QWebSocket>

#include "syncing/wan/auth/auth_token_holder.hpp"
#include "hub_message_handler.hpp"
#include "hub.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class HubWebSocket : public Hub {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HubWebSocket)

 private:

  static constexpr char const* AUTHORIZATION_HEADER = "Authorization";
  static constexpr char const* DEVICE_ID_HEADER = "x-device-id";
  static constexpr char const* HUB_BASE_URL = "hub";

 private:

  QWebSocket *webSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
  HubMessageHandler *hubMessageHandler = new HubMessageHandler(this);

 private:

  void handleTextMessage(const QString& message);

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit HubWebSocket(HubHostDevice device, QObject* parent = nullptr);

  /**
   * @brief Destroy the Client object
   */
  virtual ~HubWebSocket();

 signals:  // methods

  /**
   * @brief  On Sync Request
   */
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items) override;

 signals:  // methods

  void OnErrorOccurred(QAbstractSocket::SocketError) override;
  void OnConnected() override;
  void OnDisconnected() override;

 public:

  /**
   * @brief connect to the hub
   */
  void connect();

  /**
   * @brief disconnect from the hub
   */
  void disconnect();

  /**
   * @brief isReady
   */
  bool isReady();

  /**
   * @brief send message to the hub
   */
  void sendMessage(const QString& message) override;
};
}
