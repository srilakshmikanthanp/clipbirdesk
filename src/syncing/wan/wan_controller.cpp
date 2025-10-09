#include "wan_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
WanController::WanController(QObject *parent) : controller::Controller(parent) {}
WanController::~WanController() = default;

void WanController::synchronize(const QVector<QPair<QString, QByteArray>> &data) {
  if (m_hub && m_hub->isReady()) {
    m_hub->synchronize(data);
  }
}

void WanController::connectToHub(const syncing::wan::HubHostDevice &device) {
  if (m_hub && m_hub->isReady()) {
    throw std::runtime_error("Hub is already connected");
  }

  if (m_hub) {
    m_hub.reset();
  }

  m_hub = HubUniquePointer(new syncing::wan::HubWebSocket(device, this), [](auto p) { if (p) p->deleteLater(); });

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

  connect(
    &*m_hub, &syncing::wan::HubWebSocket::OnConnecting,
    this, &WanController::onConnecting
  );

  m_hub->connect();
}

bool WanController::isHubConnected() {
  return m_hub && m_hub->isReady();
}

void WanController::disconnectFromHub() {
  if (!m_hub || !m_hub->isReady()) {
    throw std::runtime_error("Hub is not connected");
  }

  m_hub->disconnect();
}

bool WanController::isHubAvailable() {
  return m_hub != nullptr;
}

void WanController::reconnectToHub() {
  if (!m_hub) {
    throw std::runtime_error("Hub is not available");
  }

  if (m_hub->isReady()) {
    throw std::runtime_error("Hub is already connected");
  }

  m_hub->connect();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
