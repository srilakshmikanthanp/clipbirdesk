#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QSslCertificate>
#include <QSslKey>
#include <QObject>
#include "qt6keychain/keychain.h"

#include <optional>

#include "constants/constants.hpp"
#include "syncing/wan/hub/hub_host_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::storage {
class ReadHubHostDeviceJob : public QKeychain::ReadPasswordJob {
  Q_OBJECT
public:
  using QKeychain::ReadPasswordJob::ReadPasswordJob;

  syncing::wan::HubHostDevice device() const;
};
}
