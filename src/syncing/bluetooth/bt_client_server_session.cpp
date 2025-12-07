#include "bt_client_server_session.hpp"

#include <QSslKey>

#include "constants/constants.hpp"
#include "packets/certificate_exchange_packet/certificate_exchange_packet.hpp"
#include "common/types/exceptions/exceptions.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtClientServerSession::handleCertificateExchangePacket(const packets::CertificateExchangePacket& packet) {
  this->certificate = packet.getCertificate();
  emit this->connected(this);
}

void BtClientServerSession::handleTrustedServersChanged(QMap<QString, QByteArray> servers) {
  if (m_bt_socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
    emit onTrustedStateChanged(isTrusted());
  }
}

void BtClientServerSession::handlePingTimeout() {
  using utility::functions::params::PingPacketParams;
  using utility::functions::createPacket;
  auto pingPacket = createPacket(PingPacketParams{common::types::enums::PingType::Ping});
  this->sendPacket(pingPacket);
}

void BtClientServerSession::handlePongTimeout() {
  auto lastRead = m_bt_socket->property(READ_TIME).toDateTime();
  auto now      = QDateTime::currentDateTime();
  auto diff     = lastRead.msecsTo(now);

  if (diff > constants::getAppMaxReadIdleTime()) {
    m_bt_socket->disconnectFromService();
  }
}

void BtClientServerSession::handleConnected() {
  this->m_bt_socket->write(utility::functions::createPacket({this->sslConfig.certificate}).toBytes());
  this->m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  this->m_pongTimer->start(constants::getAppMaxReadIdleTime());
}

void BtClientServerSession::handleDisconnected() {
  this->m_pingTimer->stop();
  this->m_pongTimer->stop();
  emit disconnected(this);
}

void BtClientServerSession::handleError(QBluetoothSocket::SocketError socketError) {
  emit error(this, std::make_exception_ptr(
    common::types::exceptions::ErrorCodeException(static_cast<int>(socketError), "Socket error occurred: " + std::to_string(static_cast<int>(socketError)))
  ));
}

void BtClientServerSession::handleReadyRead() {
  using utility::functions::fromQByteArray;

  m_bt_socket->setProperty(READ_TIME, QDateTime::currentDateTime());

  // get the first four bytes of the packet
  QDataStream get(m_bt_socket);

  // QByteArray to store the data
  QByteArray data;

  /// uploader QDataStream
  QDataStream put(&data, QIODevice::WriteOnly);

  // start the transaction
  get.startTransaction();

  // get the packet length
  qint32 packetLength;

  // read the packet length
  get >> packetLength;

  // write the packet length
  put << packetLength;

  // infer the read size
  auto toRead = packetLength - data.length();

  // check has enough bytes
  if (m_bt_socket->bytesAvailable() < toRead) {
    return get.rollbackTransaction();
  }

  // resize the data
  data.resize(packetLength);

  // read the data from the socket
  auto start = data.data() + data.size() - toRead;
  auto bytes = get.readRawData(start, toRead);

  // check for error
  if (bytes == -1) {
    return get.rollbackTransaction();
  }

  // update the toRead
  toRead -= bytes;

  // if toRead is not zero then return
  if (toRead) {
    return get.rollbackTransaction();
  }

  // commit the transaction
  if (!get.commitTransaction()) {
    return;
  }

  try {
    this->handleCertificateExchangePacket(fromQByteArray<packets::CertificateExchangePacket>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  if (!this->isHandshakeCompleted()) {
    this->m_bt_socket->disconnectFromService();
    return;
  }

  try {
    emit this->networkPacket(this, fromQByteArray<packets::Authentication>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  try {
    emit this->networkPacket(this, fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  try {
    emit this->networkPacket(this, fromQByteArray<packets::PingPongPacket>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  try {
    emit this->networkPacket(this, fromQByteArray<packets::InvalidRequest>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    qDebug() << e.what();
  } catch (const std::exception& e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // if no packet is found
  qDebug() << "Unknown Packet Found";
}

bool BtClientServerSession::isHandshakeCompleted() const {
  return !this->certificate.isEmpty();
}

BtClientServerSession::BtClientServerSession(
  common::trust::TrustedServers* trustedServers,
  const BtResolvedDevice& device,
  const common::types::SslConfig& sslConfig,
  QObject* parent
) : Session(device.name, parent),
    trustedServers(trustedServers),
    device(device),
    sslConfig(sslConfig) {
  QObject::connect(
    m_pingTimer, &QTimer::timeout,
    this, &BtClientServerSession::handlePingTimeout
  );

  QObject::connect(
    m_bt_socket,
    &QBluetoothSocket::connected,
    this,
    &BtClientServerSession::handleConnected
  );

  QObject::connect(
    m_bt_socket,
    &QBluetoothSocket::disconnected,
    this,
    &BtClientServerSession::handleDisconnected
  );

  QObject::connect(
    m_bt_socket,
    &QBluetoothSocket::errorOccurred,
    this,
    &BtClientServerSession::handleError
  );

  QObject::connect(
    m_bt_socket,
    &QBluetoothSocket::readyRead,
    this,
    &BtClientServerSession::handleReadyRead
  );

  QObject::connect(
    m_pongTimer, &QTimer::timeout,
    this, &BtClientServerSession::handlePongTimeout
  );

  QObject::connect(
    this->trustedServers,
    &common::trust::TrustedServers::trustedServersChanged,
    this,
    &BtClientServerSession::handleTrustedServersChanged
  );
}

BtClientServerSession::~BtClientServerSession() {
  // Nothing to do here
}

void BtClientServerSession::sendPacket(const packets::NetworkPacket& packet) {
  this->m_bt_socket->write(packet.toBytes());
}

void BtClientServerSession::disconnectFromHost() {
  this->m_bt_socket->disconnectFromService();
}

bool BtClientServerSession::isTrusted() const {
  if (!this->isHandshakeCompleted()) {
    throw std::runtime_error("Handshake not completed. Certificate not available.");
  } else {
    return this->trustedServers->isTrustedServer(this->device.name, certificate);
  }
}

QByteArray BtClientServerSession::getCertificate() const {
  if (!this->isHandshakeCompleted()) {
    throw std::runtime_error("Handshake not completed. Certificate not available.");
  } else {
    return this->certificate;
  }
}

void BtClientServerSession::connect() {
  if (this->m_bt_socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
    this->m_bt_socket->abort();
  }

  this->m_bt_socket->connectToService(
    this->device.address,
    this->device.uuid,
    QIODevice::ReadWrite
  );
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth
