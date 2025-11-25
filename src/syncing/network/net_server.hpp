#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QApplication>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QSslConfiguration>
#include <QSslServer>
#include <QSslSocket>
#include <QVector>

#include "common/types/exceptions/exceptions.hpp"
#include "common/trust/trusted_clients.hpp"
#include "net_mdns.hpp"
#include "syncing/server.hpp"
#include "syncing/session.hpp"
#include "syncing/synchronizer.hpp"
#include "syncing/network/net_server_client_session.hpp"
#include "common/types/enums/enums.hpp"
#include "utility/functions/ipconv/ipconv.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
class NetServer : public syncing::Server {
  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(NetServer)

 private:

  QSslServer* m_server         = new QSslServer(this);
  QTimer* m_pingTimer          = new QTimer(this);
  QTimer* m_pongTimer          = new QTimer(this);
  const char* READ_TIME        = "READ_TIME";
  const char* SESSION          = "SESSION";
  MdnsRegister* m_mdnsRegister = new MdnsRegister(this);
  common::trust::TrustedClients* trustedClients;
  QList<NetServerClientSession*> m_clients;

 private:
  void handlePendingConnections();
  void handleSslErrors(QSslSocket *, const QList<QSslError>& errors);
  void handleError(QAbstractSocket::SocketError socketError);
  void handleClientDisconnection();
  void handleClientReadyRead();
  void handlePingTimeout();
  void handlePongTimeout();

 public:

  explicit NetServer(const common::types::SslConfig sslConfig, common::trust::TrustedClients*, QObject *parent = nullptr);
  virtual ~NetServer();

  virtual void start() override;
  virtual void stop() override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
