#pragma once

#include <QString>

#include "session.hpp"
#include "packets/network_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class ClientServer: public QObject {
  Q_OBJECT
 protected:
  QString name;

 private:
  Q_DISABLE_COPY_MOVE(ClientServer)

 public:
  explicit ClientServer(const QString &name, QObject *parent = nullptr);
  virtual ~ClientServer();

  QString getName();

  virtual void connect() = 0;

 signals:
  void onNetworkPacket(Session* session, const packets::NetworkPacket &networkPacket);
  void onConnected(Session* session);
  void onDisconnected(Session* session);
  void onError(Session* session, std::exception_ptr eptr);
};
}
