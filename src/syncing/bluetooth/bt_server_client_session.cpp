#include "bt_server_client_session.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtServerClientSession::handleTrustedClientsChanged(QMap<QString, QByteArray> servers) {
  if (m_socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
    emit onTrustedStateChanged(isTrusted());
  }
}

BtServerClientSession::BtServerClientSession(
  const QString& name,
  const QByteArray& certificate,
  common::trust::TrustedClients* trustedClients,
  QBluetoothSocket* socket,
  QObject* parent
) : Session(name, parent), trustedClients(trustedClients), m_certificate(certificate), m_socket(socket) {
  QObject::connect(
    this->trustedClients,
    &common::trust::TrustedClients::trustedClientsChanged,
    this,
    &BtServerClientSession::handleTrustedClientsChanged
  );
}

BtServerClientSession::~BtServerClientSession() {
  this->m_socket->disconnect();
}

QBluetoothSocket* BtServerClientSession::getSocket() const {
  return m_socket;
}

void BtServerClientSession::sendPacket(const packets::NetworkPacket& packet) {
  this->m_socket->write(packet.toBytes());
}

void BtServerClientSession::disconnect() {
  this->m_socket->close();
}

bool BtServerClientSession::isTrusted() const {
  return this->trustedClients->isTrustedClient(this->getName(), m_certificate);
}

QByteArray BtServerClientSession::getCertificate() const {
  return m_certificate;
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
