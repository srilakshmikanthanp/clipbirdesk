#pragma once

#include <QSettings>

#include "common/trust/trusted_servers.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedServersQSettings : public TrustedServers {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird", this);

 private: // groups

  static constexpr const char* trustedServersGroup = "trustedServers";

 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedServersQSettings)

 private:  // qt

  Q_OBJECT

 public:
  explicit TrustedServersQSettings(QObject* parent = nullptr);
  virtual ~TrustedServersQSettings();

  virtual QList<TrustedServer> getTrustedServers()            override;
  virtual bool isTrustedServer(const TrustedServer& server)   override;
  virtual bool hasTrustedServer(const QString& name)          override;
  virtual void addTrustedServer(const TrustedServer& server)  override;
  virtual void removeTrustedServer(const QString& name)       override;
};
}
