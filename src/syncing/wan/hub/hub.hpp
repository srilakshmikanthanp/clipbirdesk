#pragma once

#include <QAbstractSocket>
#include <QWebSocket>
#include <unordered_set>

#include "syncing/synchronizer.hpp"
#include "syncing/wan/device/device_response_dto.hpp"
#include "syncing/wan/hub/hub_host_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class Hub : public Synchronizer {
 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Hub)

 private:
  std::unordered_set<DeviceResponseDto> hubDevices;

 private:
  HubHostDevice hubHostDevice;

 public:  // constructor and destructor

  /**
   * @brief Construct a new Client object
   *
   * @param parent Parent object
   */
  explicit Hub(HubHostDevice device, QObject* parent = nullptr);

  /**
   * @brief Destroy the Client object
   */
  virtual ~Hub();

 public:  // methods

  virtual void OnErrorOccurred(QAbstractSocket::SocketError) = 0;
  virtual void OnConnecting() = 0;
  virtual void OnConnected() = 0;
  virtual void OnOpened() = 0;
  virtual void OnDisconnected(QWebSocketProtocol::CloseCode code, QString reason) = 0;

 public:
  /**
   * @brief Synchronize the clipboard data
   */
  void synchronize(const QVector<QPair<QString, QByteArray>>& items) override;

  /**
   * @brief get this device
   */
  HubHostDevice getHubHostDevice() const;

  /**
   * @brief get Hub devices
   */
  std::unordered_set<DeviceResponseDto> getHubDevices();

  /**
   * @brief remove device
   */
  void removeHubDevice(const DeviceResponseDto& device);

  /**
   * @brief put device
   */
  void putHubDevice(const DeviceResponseDto& device);

  /**
   * @brief send message to the hub
   */
  virtual void sendMessage(const QString& message) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::wan
