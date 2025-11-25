#pragma once

#include <QObject>
#include <QString>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QTimer>
#include <QDateTime>

#include "common/trust/trusted_servers.hpp"
#include "common/types/ssl_config/ssl_config.hpp"
#include "packets/network_packet.hpp"
#include "packets/authentication/authentication.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "packets/invalidrequest/invalidrequest.hpp"
#include "syncing/session.hpp"
#include "syncing/network/net_resolved_device.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
class NetClientServerSession : public Session {
  Q_OBJECT

 private:
  common::trust::TrustedServers* trustedServers;
  NetResolvedDevice device;
  common::types::SslConfig sslConfig;

  QSslSocket* m_ssl_socket = new QSslSocket(this);
  QTimer* m_pingTimer = new QTimer(this);
  QTimer* m_pongTimer = new QTimer(this);
  const char* READ_TIME = "READ_TIME";

 private:
  Q_DISABLE_COPY_MOVE(NetClientServerSession)

 private:
  void handleTrustedServersChanged(QMap<QString, QByteArray> servers);
  void handleSslErrors(const QList<QSslError>& errors);
  void handlePingTimeout();
  void handlePongTimeout();
  void handleConnected();
  void handleDisconnected();
  void handleError(QAbstractSocket::SocketError socketError);
  void handleReadyRead();

 public:
  explicit NetClientServerSession(
    common::trust::TrustedServers* trustedServers,
    const NetResolvedDevice& device,
    const common::types::SslConfig& sslConfig,
    QObject* parent = nullptr
  );

  virtual ~NetClientServerSession();

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
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
