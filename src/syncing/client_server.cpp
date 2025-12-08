#include "client_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing {
ClientServer::ClientServer(const QString &name, QObject *parent): QObject(parent), name(name) {
}

ClientServer::~ClientServer() {
}

QString ClientServer::getName() {
  return name;
}
}
