#pragma once

#include <QObject>

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedClients : public QObject {
 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedClients)

 private:  // qt

  Q_OBJECT

 signals:
  void trustedClientsChanged(QMap<QString, QByteArray> clients);

 public:

  explicit TrustedClients(QObject* parent = nullptr);
  virtual ~TrustedClients();

  virtual QMap<QString, QByteArray> getTrustedClients()                             = 0;
  virtual bool isTrustedClient(const QString& name, const QByteArray& certificate)  = 0;
  virtual bool hasTrustedClient(const QString& name)                                = 0;
  virtual void addTrustedClient(const QString& name, const QByteArray& certificate) = 0;
  virtual void removeTrustedClient(const QString& name)                             = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::common::trust
