#pragma once

#include <QSettings>

#include "common/trust/trusted_clients.hpp"

namespace srilakshmikanthanp::clipbirdesk::common::trust {
class TrustedClientsQSettings : public TrustedClients {
 private:  // settings

  QSettings *settings = new QSettings("srilakshmikanthanp", "clipbird", this);

 private: // groups

  static constexpr const char* trustedClientsGroup = "trustedClients";

 private:  // constructor

  Q_DISABLE_COPY_MOVE(TrustedClientsQSettings)

 private:  // qt

  Q_OBJECT

 public:
  explicit TrustedClientsQSettings(QObject* parent = nullptr);
  virtual ~TrustedClientsQSettings();

  QMap<QString, QByteArray> getTrustedClients() override;
  bool isTrustedClient(const QString& name, const QByteArray& certificate) override;
  bool hasTrustedClient(const QString& name) override;
  void addTrustedClient(const QString& name, const QByteArray& certificate) override;
  void removeTrustedClient(const QString& name) override;
};
}
