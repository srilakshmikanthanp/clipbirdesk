#pragma once

#include <QObject>
#include <QString>

#include "common/types/ssl_config/ssl_config.hpp"
#include "packets/authentication/authentication.hpp"
#include "packets/invalidrequest/invalid_request_exception.hpp"
#include "packets/invalidrequest/invalidrequest.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "syncing/manager/host_manager.hpp"
#include "syncing/client_server.hpp"
#include "syncing/session.hpp"
#include "packets/network_packet.hpp"
#include "syncing/client_server_browser.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class ClientManager : public HostManager {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(ClientManager)

 private:
  void handleAuthenticationPacket(Session* session, const packets::Authentication& packet);
  void handleInvalidRequestPacket(Session* session, const packets::InvalidRequest& packet);
  void handleSyncingPacket(Session* session, const packets::SyncingPacket& packet);
  void handlePingPongPacket(Session* session, const packets::PingPongPacket& packet);
  void handleServerFound(ClientServer *server);
  void handleServerGone(ClientServer *server);
  void handleBrowsingStarted();
  void handleBrowsingStopped();
  void handleBrowsingStartFailed(std::exception_ptr eptr);
  void handleBrowsingStopFailed(std::exception_ptr eptr);

 public:
  void handleNetworkPacket(Session* session, const packets::NetworkPacket& networkPacket);
  void handleConnected(Session *session);
  void handleDisconnected(Session *session);
  void handleError(Session *session, std::exception_ptr eptr);

 private:
  ClientServerBrowser* clientServerBrowser = nullptr;
  Session* session                         = nullptr;

 public:
  explicit ClientManager(QObject* parent = nullptr);
  virtual ~ClientManager();

  virtual void synchronize(const QVector<QPair<QString, QByteArray>>& items) override;
  virtual void start(bool useBluetooth) override;
  virtual void stop() override;

  void connectToServer(ClientServer* server);

 signals:
  void browsingStartFailed(std::exception_ptr eptr);
  void browsingStopFailed(std::exception_ptr eptr);
  void serverFound(ClientServer *server);
  void serverGone(ClientServer *server);
  void browsingStarted();
  void browsingStopped();
  void connected(Session *session);
  void disconnected(Session *session);
  void errorOccurred(Session *session, std::exception_ptr eptr);
};
}
