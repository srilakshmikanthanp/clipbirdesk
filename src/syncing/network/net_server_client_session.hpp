#pragma once

#include <QObject>
#include <QSslSocket>

#include "common/trust/trusted_clients.hpp"
#include "syncing/session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
class NetServerClientSession : public Session {
  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(NetServerClientSession)

 private:

  common::trust::TrustedClients* trustedClients;
  QByteArray m_certificate;
  QSslSocket* m_socket;

 private:
  void handleTrustedClientsChanged(QMap<QString, QByteArray> servers);

 public:

  NetServerClientSession(
    const QString& name,
    const QByteArray& certificate,
    common::trust::TrustedClients* trustedClients,
    QSslSocket* socket,
    QObject* parent = nullptr
  );

  ~NetServerClientSession() override;

  QSslSocket* getSocket() const;

  void sendPacket(const packets::NetworkPacket& packet) override;
  void disconnect() override;
  bool isTrusted() const override;
  QByteArray getCertificate() const override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
