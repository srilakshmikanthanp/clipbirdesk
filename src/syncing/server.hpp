#pragma once

#include <QObject>

#include "common/types/ssl_config/ssl_config.hpp"
#include "session.hpp"
#include "packets/network_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class Server : public QObject {
  Q_OBJECT

 private:
  Q_DISABLE_COPY_MOVE(Server)

 protected:
  common::types::SslConfig sslConfig;

 public:
  explicit Server(const common::types::SslConfig sslConfig, QObject *parent = nullptr);
  virtual ~Server();

  virtual void start() = 0;
  virtual void stop()  = 0;

signals:
  void onClientDisconnected(Session* client);
  void onClientConnected(Session* client);
  void onClientError(Session* client, std::exception_ptr error);
  void onNetworkPacket(Session* client, const packets::NetworkPacket &networkPacket);
  void onServiceRegistered();
  void onServiceUnregistered();
  void onServiceRegistrationFailed(std::exception_ptr error);
  void onServiceUnregistrationFailed(std::exception_ptr error);
};
}
