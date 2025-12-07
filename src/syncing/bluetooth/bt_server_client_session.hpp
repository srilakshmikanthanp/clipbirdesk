#pragma once

#include <QObject>
#include <QBluetoothSocket>

#include "common/trust/trusted_clients.hpp"
#include "syncing/session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtServerClientSession : public Session {
  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(BtServerClientSession)

 private:

  common::trust::TrustedClients* trustedClients;
  QByteArray m_certificate;
  QBluetoothSocket* m_socket;

 private:
  void handleTrustedClientsChanged(QMap<QString, QByteArray> servers);

 public:

  BtServerClientSession(
    const QString& name,
    const QByteArray& certificate,
    common::trust::TrustedClients* trustedClients,
    QBluetoothSocket* socket,
    QObject* parent = nullptr
  );

  ~BtServerClientSession() override;

  QBluetoothSocket* getSocket() const;

  void sendPacket(const packets::NetworkPacket& packet) override;
  void disconnectFromHost() override;
  bool isTrusted() const override;
  QByteArray getCertificate() const override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
