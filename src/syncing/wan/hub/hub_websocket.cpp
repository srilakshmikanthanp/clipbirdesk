#include "constants/constants.hpp"
#include "hub_websocket.hpp"

#include "hub_message_clipboard_dispatch_payload_handler.hpp"
#include "hub_message_device_added_payload_handler.hpp"
#include "hub_message_device_removed_payload_handler.hpp"
#include "hub_message_device_updated_payload_handler.hpp"
#include "hub_message_nonce_challenge_request_payload_handler.hpp"
#include "hub_message_devices_payload_handler.hpp"
#include "hub_message_nonce_challenge_completed_payload_handler.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
/**
 *  @brief Construct a new Hub Web Socket Impl object
 */
HubWebSocket::HubWebSocket(HubHostDevice hubHostDevice, QObject* parent) : Hub(hubHostDevice, parent) {;
  QObject::connect(webSocket, &QWebSocket::textMessageReceived, this, &HubWebSocket::handleTextMessage);
  QObject::connect(webSocket, &QWebSocket::errorOccurred, this, &HubWebSocket::OnErrorOccurred);
  QObject::connect(webSocket, &QWebSocket::disconnected, this, &HubWebSocket::OnDisconnected);

  hubMessageHandler->setPayloadHandler(new HubMessageClipboardDispatchPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageDeviceAddedPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageDeviceRemovedPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageDeviceUpdatedPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageDevicesPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageNonceChallengeRequestPayloadHandler(this));
  hubMessageHandler->setPayloadHandler(new HubMessageNonceChallengeCompletedPayloadHandler(this));
}

/**
 * @brief Destroy the Hub Web Socket Impl object
 */
HubWebSocket::~HubWebSocket() = default;

/**
 * @brief Handle text message
 */
void HubWebSocket::handleTextMessage(const QString& message) {
  try {
    nlohmann::json j = nlohmann::json::parse(message.toUtf8());
    HubMessage msg = j.template get<HubMessage>();
    this->hubMessageHandler->handle(this, msg);
  } catch (const std::exception& e) {
    qWarning() << "Failed to handle message:" << e.what();
  }
}

/**
 * @brief Connect to the hub
 */
void HubWebSocket::connect() {
  QNetworkRequest request(QUrl(QString("%1/%2").arg(constants::getClipbirdWebSocketUrl()).arg(HUB_BASE_URL)));
  request.setRawHeader(AUTHORIZATION_HEADER, QString("Bearer %1").arg(AuthTokenHolder::instance().getAuthTokenOrThrow().token).toUtf8());
  request.setRawHeader(DEVICE_ID_HEADER, getHubHostDevice().id.toUtf8());
  webSocket->open(request);
}

/**
 * @brief disconnect from the hub
 */
void HubWebSocket::disconnect() {
  webSocket->close();
}

/**
 * @brief isReady
 */
bool HubWebSocket::isReady() {
  return webSocket->isValid();
}

/**
 * @brief send message to the hub
 */
void HubWebSocket::sendMessage(const QString& message) {
  if (webSocket->isValid()) {
    webSocket->sendTextMessage(message);
  } else {
    throw std::runtime_error("WebSocket is not ready");
  }
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
