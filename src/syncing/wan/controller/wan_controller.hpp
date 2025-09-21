#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// project headers
#include "controller/controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class WanController : public Controller {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(WanController)

 private:

  std::optional<syncing::wan::HubWebSocket> m_hub;

 signals:

  void OnHubErrorOccurred(QAbstractSocket::SocketError);
  void OnHubConnected();
  void OnHubDisconnected();

 public:  // Constructors and Destructors

  WanController(QObject *parent = nullptr);
  virtual ~WanController() override;

  void synchronize(const QVector<QPair<QString, QByteArray>> &data);

  void connectToHub(const syncing::wan::HubHostDevice &device);
  bool isHubConnected();
  void disconnectFromHub();
};
}
