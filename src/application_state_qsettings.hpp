#pragma once

#include <QSettings>
#include "application_state.hpp"

namespace srilakshmikanthanp::clipbirdesk {
class ApplicatiionStateQSettings : public ApplicatiionState {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird", this);

 private: // groups

  static constexpr const char* applicatiionStateGroup = "applicatiionState";

 private: // keys

  static constexpr const char* certificateKey = "certificate";
  static constexpr const char* keyKey = "key";
  static constexpr const char* isServerKey = "isServer";
  static constexpr const char* useBluetoothKey = "useBluetooth";

 private:  // constructor

  Q_DISABLE_COPY_MOVE(ApplicatiionStateQSettings)

 private:  // qt

  Q_OBJECT

 public:
  explicit ApplicatiionStateQSettings(QObject* parent = nullptr);
  virtual ~ApplicatiionStateQSettings();

  void setHostSslConfig(const std::optional<common::types::SslConfig>& sslConfig);
  std::optional<common::types::SslConfig> getHostSslConfig() const override;

  bool getIsServer() const override;
  void setIsServer(bool isServer) override;

  bool shouldUseBluetooth() const override;
  void setUseBluetooth(bool useBluetooth) override;
};
}
