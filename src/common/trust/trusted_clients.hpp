#pragma once

#include <QObject>
#include <QList>
#include "trusted_client.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedClients : public QObject {
 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedClients)

 private:  // qt

  Q_OBJECT

 signals:
  void trustedClientsChanged(QList<TrustedClient> clients);

 public:

  explicit TrustedClients(QObject* parent = nullptr);
  virtual ~TrustedClients();

  virtual QList<TrustedClient> getTrustedClients()            = 0;
  virtual bool isTrustedClient(const TrustedClient& client)   = 0;
  virtual bool hasTrustedClient(const QString& name)          = 0;
  virtual void addTrustedClient(const TrustedClient& client)  = 0;
  virtual void removeTrustedClient(const QString& name)       = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
