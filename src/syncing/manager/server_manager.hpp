#pragma once

#include <QObject>
#include <QString>
#include <QVector>

#include "common/types/ssl_config/ssl_config.hpp"
#include "packets/authentication/authentication.hpp"
#include "packets/invalidrequest/invalidrequest.hpp"
#include "packets/syncingpacket/syncingpacket.hpp"
#include "packets/pingpongpacket/pingpongpacket.hpp"
#include "syncing/manager/host_manager.hpp"
#include "syncing/server.hpp"
#include "syncing/server_factory.hpp"
#include "syncing/session.hpp"
#include "packets/network_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class ServerManager : public HostManager {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(ServerManager)

 private:
  void onSyncingPacket(Session* session, const packets::SyncingPacket& packet);
  void onPingPongPacket(Session* session, const packets::PingPongPacket& packet);
  void onClientDisconnected(Session* session);
  void onClientConnected(Session* session);
  void onClientError(Session* session, std::exception_ptr eptr);
  void onServiceRegistered();
  void onServiceUnregistered();
  void onServiceRegistrationFailed(std::exception_ptr eptr);
  void onServiceUnregistrationFailed(std::exception_ptr eptr);
  void onNetworkPacket(Session* session, const packets::NetworkPacket& networkPacket);

 private:
  Server* server = nullptr;
  QVector<Session*> clients;

 public:
  explicit ServerManager(QObject* parent = nullptr);
  virtual ~ServerManager();

  virtual void synchronize(const QVector<QPair<QString, QByteArray>>& items) override;
  virtual void start(bool useBluetooth) override;
  virtual void stop() override;

 signals:
  void clientDisconnected(Session* session);
  void clientConnected(Session* session);
  void serviceRegistered();
  void serviceUnregistered();
  void serviceRegisteringFailed(std::exception_ptr eptr);
  void serviceUnregisteringFailed(std::exception_ptr eptr);
  void errorOccurred(Session* session, std::exception_ptr eptr);
};
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
