#pragma once

#include <optional>
#include <QObject>

#include "common/types/ssl_config/ssl_config.hpp"

namespace srilakshmikanthanp::clipbirdesk {
class ApplicatiionState : public QObject {
 private:  // constructor

  Q_DISABLE_COPY_MOVE(ApplicatiionState)

 private:  // qt

  Q_OBJECT

 signals:
  void onHostSSlConfigChanged(const std::optional<common::types::SslConfig>& sslConfig);
  void onIsServerChanged(bool isServer);
  void shouldUseBluetoothChanged(bool useBluetooth);

 public:
  explicit ApplicatiionState(QObject* parent = nullptr);
  virtual ~ApplicatiionState();

  virtual void setHostSslConfig(const std::optional<common::types::SslConfig>& sslConfig) = 0;
  virtual std::optional<common::types::SslConfig> getHostSslConfig() const = 0;

  virtual bool getIsServer() const = 0;
  virtual void setIsServer(bool isServer) = 0;

  virtual bool shouldUseBluetooth() const = 0;
  virtual void setUseBluetooth(bool useBluetooth) = 0;
};
}
