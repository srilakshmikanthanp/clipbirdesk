#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QApplication>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QVector>
#include <QBluetoothServer>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>
#include <QTimer>
#include <QBluetoothLocalDevice>
#include <QVariant>

#include "common/types/exceptions/exceptions.hpp"
#include "common/trust/trusted_clients.hpp"
#include "constants/constants.hpp"
#include "syncing/server.hpp"
#include "syncing/session.hpp"
#include "syncing/synchronizer.hpp"
#include "syncing/bluetooth/bt_server_client_session.hpp"
#include "common/types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
class BtServer : public syncing::Server {
  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(BtServer)

 private:

  QBluetoothServer* m_server   = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
  QBluetoothServiceInfo serviceInfo;
  QTimer* m_pingTimer          = new QTimer(this);
  QTimer* m_pongTimer          = new QTimer(this);
  const char* READ_TIME        = "READ_TIME";
  const char* SESSION          = "SESSION";
  common::trust::TrustedClients* trustedClients;
  QList<BtServerClientSession*> m_clients;

 private:
  void handleCertificateExchangePacket(const packets::CertificateExchangePacket& packet);
  void handlePendingConnections();
  void handleSslErrors(QBluetoothSocket *, const QList<QSslError>& errors);
  void handleError(QAbstractSocket::SocketError socketError);
  void handleClientDisconnection();
  void handleClientReadyRead();
  void handlePingTimeout();
  void handlePongTimeout();

 public:

  explicit BtServer(const common::types::SslConfig sslConfig, common::trust::TrustedClients*, QObject *parent = nullptr);
  virtual ~BtServer();

  virtual void start() override;
  virtual void stop() override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
