
#pragma once

#include <QObject>
#include <QString>
#include <QBluetoothSocket>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QTimer>
#include <QDateTime>

#include "common/trust/trusted_servers.hpp"
#include "common/types/exceptions/exceptions.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "packets/network_packet.hpp"
#include "packets/authentication/authentication.hpp"
#include "packets/certificate_exchange_packet/certificate_exchange_packet.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "packets/invalidrequest/invalidrequest.hpp"
#include "syncing/session.hpp"
#include "syncing/bluetooth/bt_resolved_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtClientServerSession : public Session {
  Q_OBJECT

 private:
  common::trust::TrustedServers* trustedServers;
  BtResolvedDevice device;
  common::types::SslConfig sslConfig;

  QBluetoothSocket* m_bt_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
  QByteArray certificate;
  QTimer* m_pingTimer = new QTimer(this);
  QTimer* m_pongTimer = new QTimer(this);
  const char* READ_TIME = "READ_TIME";

 private:
  Q_DISABLE_COPY_MOVE(BtClientServerSession)

 private:
  void handleCertificateExchangePacket(const packets::CertificateExchangePacket& packet);
  void handleTrustedServersChanged(QMap<QString, QByteArray> servers);
  void handlePingTimeout();
  void handlePongTimeout();
  void handleConnected();
  void handleDisconnected();
  void handleError(QBluetoothSocket::SocketError error);
  void handleReadyRead();

 private:
  bool isHandshakeCompleted() const;

 public:
  explicit BtClientServerSession(
    common::trust::TrustedServers* trustedServers,
    const BtResolvedDevice& device,
    const common::types::SslConfig& sslConfig,
    QObject* parent = nullptr
  );

  virtual ~BtClientServerSession();

  virtual void sendPacket(const packets::NetworkPacket& packet) override;
  virtual void disconnect() override;
  virtual bool isTrusted() const override;
  virtual QByteArray getCertificate() const override;

  void connect();

 signals:
  void connected(Session* session);
  void disconnected(Session* session);
  void error(Session*, std::exception_ptr eptr);
  void networkPacket(Session*, const packets::NetworkPacket& packet);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::btwork
