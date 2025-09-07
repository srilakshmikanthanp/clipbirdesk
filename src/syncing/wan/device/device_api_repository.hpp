#pragma once

#include <QObject>
#include <QString>
#include <QFuture>

#include "constants/constants.hpp"
#include "device_api_client.hpp"
#include "device_repository.hpp"
#include "device_request_dto.hpp"
#include "device_response_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class DeviceApiRepository : public DeviceRepository {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DeviceApiRepository)

 private:

  DeviceApiClient* m_apiClient;

 public:

  /**
   * @brief Construct a new DeviceRepository object
   *
   * @param parent Parent object
   */
  explicit DeviceApiRepository(DeviceApiClient *apiClient, QObject* parent = nullptr);

  /**
   * @brief Create a new device
   */
  QFuture<DeviceResponseDto> createDevice(DeviceRequestDto dto) override;

  /**
   * @brief Update device by ID
   */
  QFuture<DeviceResponseDto> updateDevice(const QString& id, DeviceRequestDto dto) override;

  /**
   * @brief Destroy the DeviceRepository object
   */
  virtual ~DeviceApiRepository();
};
}
