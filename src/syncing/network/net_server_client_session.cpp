#include "net_server_client_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
void NetServerClientSession::handleTrustedClientsChanged(QMap<QString, QByteArray> servers) {
  if (m_socket->state() == QAbstractSocket::ConnectedState) {
    emit onTrustedStateChanged(isTrusted());
  }
}

NetServerClientSession::NetServerClientSession(
  const QString& name,
  const QByteArray& certificate,
  common::trust::TrustedClients* trustedClients,
  QSslSocket* socket,
  QObject* parent
) : Session(name, parent), trustedClients(trustedClients), m_certificate(certificate), m_socket(socket) {
  connect(
    this->trustedClients,
    &common::trust::TrustedClients::trustedClientsChanged,
    this,
    &NetServerClientSession::handleTrustedClientsChanged
  );
}

NetServerClientSession::~NetServerClientSession() {
  this->m_socket->disconnectFromHost();
}

QSslSocket* NetServerClientSession::getSocket() const {
  return m_socket;
}

void NetServerClientSession::sendPacket(const packets::NetworkPacket& packet) {
  const auto data = packet.toBytes();
  qint32 wrote    = 0L;

  while (wrote < data.size()) {
    auto bytes = this->m_socket->write(data.data() + wrote, data.size() - wrote);
    if (bytes == -1) break;
    wrote = wrote + bytes;
  }

  if (wrote != data.size()) {
    qErrnoWarning("Error while writing to the socket");
  }

  this->m_socket->flush();
}

void NetServerClientSession::disconnectFromHost() {
  this->m_socket->disconnectFromHost();
}

bool NetServerClientSession::isTrusted() const {
  return this->trustedClients->isTrustedClient(this->getName(), m_certificate);
}

QByteArray NetServerClientSession::getCertificate() const {
  return m_certificate;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
