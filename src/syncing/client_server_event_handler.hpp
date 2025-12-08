#pragma once

#include <QString>

#include "session.hpp"
#include "syncing/manager/host_manager.hpp"
#include "packets/network_packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
class ClientServerEventHandler: public HostManager {
  Q_DISABLE_COPY_MOVE(ClientServerEventHandler)

 public:
  explicit ClientServerEventHandler(QObject * parent = nullptr);
  virtual ~ClientServerEventHandler();

  virtual void handleNetworkPacket(Session* session, const packets::NetworkPacket& networkPacket) = 0;
  virtual void handleConnected(Session *session) = 0;
  virtual void handleDisconnected(Session *session) = 0;
  virtual void handleError(Session *session, std::exception_ptr eptr) = 0;
};
}
