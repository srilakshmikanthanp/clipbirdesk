#include "clipbird_qml_client_server.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml {

ClipbirdQmlClientServer::ClipbirdQmlClientServer(syncing::ClientServer* clientServer, QObject* parent): QObject(parent), m_clientServer(clientServer) {
}

ClipbirdQmlClientServer::~ClipbirdQmlClientServer() = default;

QString ClipbirdQmlClientServer::getName() const {
  return m_clientServer->getName();
}

syncing::ClientServer* ClipbirdQmlClientServer::getClientServer() const {
  return m_clientServer;
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::qml
