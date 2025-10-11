#pragma once

#include <QTimer>
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

  QTimer *pingTimer = new QTimer(this);
  QTimer *pongTimer = new QTimer(this);
  QElapsedTimer lastPong;
  QWebSocket *webSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
  HubMessageHandler *hubMessageHandler = new HubMessageHandler(this);

 private:
  QTimer* reconnectTimer = new QTimer(this);
  const double backOffFactor = 2.0;
  const int baseDelayMs = 2000;
  const int maxDelayMs = 60000;
  int reconnectAttempts = 0;

 private:

  void handleErrorOccured(QAbstractSocket::SocketError error);
  void handleTextMessage(const QString& message);
  void handleConnected();
  void handleDisconnected();
  void handlePingTimeout();
  void handlePong();
  void handlePongTimeout();

  void scheduleReconnect();
  void resetReconnectSchedule();

  void makeConnection();

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
  void OnConnecting() override;
  void OnConnected() override;
  void OnDisconnected(QWebSocketProtocol::CloseCode code, QString reason) override;

 public:

  void connect();
  void disconnect();
  bool isReady();
  void sendMessage(const QString& message) override;
};
}
