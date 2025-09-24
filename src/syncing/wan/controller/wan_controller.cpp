#include "wan_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
WanController::WanController(QObject *parent) : Controller(parent) {}
WanController::~WanController() = default;

void WanController::synchronize(const QVector<QPair<QString, QByteArray>> &data) {
  if (m_hub.has_value() && m_hub->isReady()) {
    m_hub->synchronize(data);
  }
}

void WanController::connectToHub(const syncing::wan::HubHostDevice &device) {
  if (m_hub.has_value() && m_hub->isReady()) {
    throw std::runtime_error("Hub is already connected");
  }

  m_hub.emplace(device, this);

  connect(
    &*m_hub, &syncing::wan::HubWebSocket::OnErrorOccurred,
    this, &WanController::OnHubErrorOccurred
  );

  connect(
    &*m_hub, &syncing::wan::HubWebSocket::OnConnected,
    this, &WanController::OnHubConnected
  );

  connect(
    &*m_hub, &syncing::wan::HubWebSocket::OnDisconnected,
    this, &WanController::OnHubDisconnected
  );

  connect(
    &*m_hub, &syncing::wan::HubWebSocket::OnSyncRequest,
    this, &WanController::OnSyncRequest
  );

  m_hub->connect();
}

bool WanController::isHubConnected() {
  return m_hub.has_value() && m_hub->isReady();
}

void WanController::disconnectFromHub() {
  if (!m_hub.has_value() || !m_hub->isReady()) {
    throw std::runtime_error("Hub is not connected");
  }

  m_hub->disconnect();
}
}
