#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// project headers
#include "controller/controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class WanController : public controller::Controller {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(WanController)

 private:
  using HubUniquePointer = std::unique_ptr<syncing::wan::HubWebSocket, std::function<void(syncing::wan::HubWebSocket*)>>;
  HubUniquePointer m_hub;

 private:
  void handleDisconnected(QWebSocketProtocol::CloseCode code, QString reason);
  void handleErrorOccurred(QAbstractSocket::SocketError);

 signals:

  void OnHubDisconnected(QWebSocketProtocol::CloseCode code, QString reason);
  void OnHubErrorOccurred(QAbstractSocket::SocketError);
  void OnHubConnected();
  void onConnecting();
  void OnSyncRequest(QVector<QPair<QString, QByteArray>> items);

 public:  // Constructors and Destructors

  WanController(QObject *parent = nullptr);
  virtual ~WanController() override;

  void synchronize(const QVector<QPair<QString, QByteArray>> &data);

  void connectToHub(const syncing::wan::HubHostDevice &device);
  bool isHubConnected();
  void disconnectFromHub();
};
}
