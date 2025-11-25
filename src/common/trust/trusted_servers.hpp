#pragma once

#include <QObject>

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedServers : public QObject {
 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedServers)

 private:  // qt

  Q_OBJECT

 signals:
  void trustedServersChanged(QMap<QString, QByteArray> servers);

 public:
  explicit TrustedServers(QObject* parent = nullptr);
  virtual ~TrustedServers();

  virtual QMap<QString, QByteArray> getTrustedServers() = 0;
  virtual bool isTrustedServer(const QString& name, const QByteArray& certificate) = 0;
  virtual bool hasTrustedServer(const QString& name) = 0;
  virtual void addTrustedServer(const QString& name, const QByteArray& certificate) = 0;
  virtual void removeTrustedServer(const QString& name) = 0;
};
}
