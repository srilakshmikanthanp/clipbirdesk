#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QSslCertificate>
#include <QSslKey>
#include <QObject>
#include "qt6keychain/keychain.h"

#include <optional>

#include "constants/constants.hpp"
#include "syncing/wan/auth/auth_token_dto.hpp"
#include "syncing/wan/hub/hub_host_device.hpp"
#include "read_auth_token_dto_job.hpp"
#include "read_hub_host_device_job.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
class SecureStorage : public QObject {
 private: // keys

  const char *hubHostDeviceKey = "hubHostDevice";
  const char *hubJwtTokenKey = "hubJwtToken";

 private:  // qt

  Q_OBJECT

 private:  // constructor

  Q_DISABLE_COPY_MOVE(SecureStorage)

 private:  // constructor & destructor

  /**
   * @brief Construct a new SQLStore object
   */
  SecureStorage(QObject *parent = nullptr);

  /**
   * @brief Destroy the SQLStore object
   */
  virtual ~SecureStorage();

  public:  // methods

  /**
   * @brief Set the Hub JWT token
   */
  Q_INVOKABLE QKeychain::WritePasswordJob* setHubAuthTokenToken(const syncing::wan::AuthTokenDto &dto);

  /**
   * @brief Get the Hub JWT token
   */
  Q_INVOKABLE ReadAuthTokenDtoJob* getHubAuthToken();

  /**
   * @brief Remove the Hub JWT token
   */
  Q_INVOKABLE QKeychain::DeletePasswordJob* removeHubJwtToken();

  /**
   * @brief set the Hub Host Device
   */
  Q_INVOKABLE QKeychain::WritePasswordJob* setHubHostDevice(const syncing::wan::HubHostDevice &device);

  /**
   * @brief Get the Hub Host Device
   */
  Q_INVOKABLE ReadHubHostDeviceJob* getHubHostDevice();

  /**
   * @brief Remove the Hub Host Device
   */
  Q_INVOKABLE QKeychain::DeletePasswordJob* removeHubHostDevice();

  /**
   * @brief Instance of the storage
   */
  static SecureStorage &instance();
};
}
