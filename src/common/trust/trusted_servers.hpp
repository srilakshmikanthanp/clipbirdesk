#pragma once

#include <QObject>
#include <QList>
#include "trusted_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedServers : public QObject {
 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedServers)

 private:  // qt

  Q_OBJECT

 signals:
  void trustedServersChanged(QList<TrustedServer> servers);

 public:
  explicit TrustedServers(QObject* parent = nullptr);
  virtual ~TrustedServers();

  virtual QList<TrustedServer> getTrustedServers()            = 0;
  virtual bool isTrustedServer(const TrustedServer& server)   = 0;
  virtual bool hasTrustedServer(const QString& name)          = 0;
  virtual void addTrustedServer(const TrustedServer& server)  = 0;
  virtual void removeTrustedServer(const QString& name)       = 0;
};
}
