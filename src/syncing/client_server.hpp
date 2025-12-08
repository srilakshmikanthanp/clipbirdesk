#pragma once

#include <QString>

#include "session.hpp"
#include "packets/network_packet.hpp"
#include "syncing/client_server_event_handler.hpp"

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

  virtual void connect(syncing::ClientServerEventHandler *handler) = 0;
};
}
