#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QObject>
#include <QFuture>
#include <QPromise>

#include "store/secure_storage.hpp"

// project headers
#include "syncing/wan/wan_controller.hpp"
#include "syncing/wan/device/device_repository.hpp"
#include "syncing/wan/device/device_api_repository.hpp"
#include "syncing/wan/device/device_api_client.hpp"
#include "syncing/wan/device/device_request_dto.hpp"
#include "syncing/wan/device/device_response_dto.hpp"
#include "syncing/wan/device/device_type.hpp"
#include "syncing/wan/auth/auth_repository.hpp"
#include "syncing/wan/auth/auth_api_repository.hpp"
#include "syncing/wan/auth/auth_api_client.hpp"
#include "syncing/wan/auth/auth_token_holder.hpp"
#include "utility/functions/crypto/crypto.hpp"
#include "utility/functions/keychain/keychain.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::wan {
class WanViewModel : public QObject {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(WanViewModel)

 private:

  DeviceRepository *deviceRepository = new DeviceApiRepository(new DeviceApiClient(this), this);
  AuthRepository *authRepository = new AuthApiRepository(new AuthApiClient(this), this);
  syncing::wan::WanController *wanController;

 public:  // Constructors and Destructors

  WanViewModel(syncing::wan::WanController* wanController, QObject *parent = nullptr);
  virtual ~WanViewModel();

  QFuture<void> connectToHub();
};
}
