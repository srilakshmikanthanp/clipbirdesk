#pragma once

#include <QObject>
#include <QString>
#include <QFuture>

#include "constants/constants.hpp"
#include "device_request_dto.hpp"
#include "device_response_dto.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class DeviceApiClient : public QObject {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(DeviceApiClient)

 private:

  static constexpr const char* BASE_URL = "devices";

 public:  // constructor and destructor

  /**
   * @brief Construct a new DeviceApiClient object
   *
   * @param parent Parent object
   */
  explicit DeviceApiClient(QObject* parent = nullptr);

  /**
   * @brief Destroy the DeviceApiClient object
   */
  virtual ~DeviceApiClient();

 public: // methods

  /**
   * @brief Create a new device
   */
  QFuture<DeviceResponseDto> createDevice(DeviceRequestDto dto);

  /**
   * @brief Update device by ID
   */
  QFuture<DeviceResponseDto> updateDevice(const QString& id, DeviceRequestDto dto);
};
}
