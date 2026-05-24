#pragma once

#include <optional>
#include <QSettings>
#include "common/types/ssl_config/ssl_config.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::types {
class SslConfigProvider : public QObject {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird", this);

 private: // groups

  static constexpr const char* sslConfigGroup = "sslConfig";

 private: // keys

  static constexpr const char* certificateKey = "certificate";
  static constexpr const char* keyKey = "key";

 private:  // constructor

  Q_DISABLE_COPY_MOVE(SslConfigProvider)

 private:  // qt

  Q_OBJECT

 signals:
  void onHostSSlConfigChanged(const std::optional<common::types::SslConfig>& sslConfig);

 public:
  explicit SslConfigProvider(QObject* parent = nullptr);
  virtual ~SslConfigProvider();

  void setHostSslConfig(const std::optional<common::types::SslConfig>& sslConfig);
  std::optional<common::types::SslConfig> getHostSslConfig() const;
};
}
