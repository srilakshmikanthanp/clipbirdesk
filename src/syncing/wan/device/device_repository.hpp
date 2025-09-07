#pragma once

#include <QObject>
#include <QString>
#include <QFuture>

#include "constants/constants.hpp"
#include "device_request_dto.hpp"
#include "device_response_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class DeviceRepository : public QObject {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DeviceRepository)

 public:

  /**
   * @brief Construct a new DeviceRepository object
   *
   * @param parent Parent object
   */
  explicit DeviceRepository(QObject* parent = nullptr);

  /**
   * @brief Create a new device
   */
  virtual QFuture<DeviceResponseDto> createDevice(DeviceRequestDto dto) = 0;

  /**
   * @brief Update device by ID
   */
  virtual QFuture<DeviceResponseDto> updateDevice(const QString& id, DeviceRequestDto dto) = 0;

  /**
   * @brief Destroy the DeviceRepository object
   */
  virtual ~DeviceRepository();
};
}
