#include "net_client_server_session.hpp"

#include <QSslKey>

#include "constants/constants.hpp"
#include "common/types/exceptions/exceptions.hpp"
#include "utility/functions/nbytes/nbytes.hpp"
#include "utility/functions/packet/packet.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
void NetClientServerSession::handleTrustedServersChanged(QMap<QString, QByteArray> servers) {
  if (m_ssl_socket->state() == QAbstractSocket::ConnectedState) {
    emit onTrustedStateChanged(isTrusted());
  }
}

void NetClientServerSession::handleSslErrors(const QList<QSslError>& errors) {
  QList<QSslError::SslError> ignoredErrors;

  ignoredErrors.append(QSslError::SelfSignedCertificate);
  ignoredErrors.append(QSslError::HostNameMismatch);
  ignoredErrors.append(QSslError::UnspecifiedError);
  ignoredErrors.append(QSslError::CertificateUntrusted);

  auto errorsCopy = errors;

  auto itr = std::remove_if(errorsCopy.begin(), errorsCopy.end(), [&](auto error) {
    return ignoredErrors.contains(error.error());
  });

  errorsCopy.erase(itr, errorsCopy.end());

  for (auto error : errorsCopy) {
    qWarning() << std::to_string(error.error()) + " : " + error.errorString().toStdString();
  }

  if (!errorsCopy.isEmpty()) {
    return this->m_ssl_socket->abort();
  }

  const auto certificate = this->m_ssl_socket->peerCertificate();
  auto name = certificate.subjectInfo(QSslCertificate::CommonName).constFirst();
  if (certificate.isNull() || name.isEmpty()) {
    return m_ssl_socket->abort();
  }

  this->m_ssl_socket->ignoreSslErrors();
}

void NetClientServerSession::handlePingTimeout() {
  using utility::functions::params::PingPacketParams;
  using utility::functions::createPacket;
  auto pingPacket = createPacket(PingPacketParams{common::types::enums::PingType::Ping});
  this->sendPacket(pingPacket);
}

void NetClientServerSession::handlePongTimeout() {
  auto lastRead = m_ssl_socket->property(READ_TIME).toDateTime();
  auto now      = QDateTime::currentDateTime();
  auto diff     = lastRead.msecsTo(now);

  if (diff > constants::getAppMaxReadIdleTime()) {
    m_ssl_socket->disconnectFromHost();
  }
}

void NetClientServerSession::handleConnected() {
  this->m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  this->m_pongTimer->start(constants::getAppMaxReadIdleTime());
  emit connected(this);
}

void NetClientServerSession::handleDisconnected() {
  this->m_pingTimer->stop();
  this->m_pongTimer->stop();
  emit disconnected(this);
}

void NetClientServerSession::handleError(QAbstractSocket::SocketError socketError) {
  emit error(this, std::make_exception_ptr(
    common::types::exceptions::ErrorCodeException(static_cast<int>(socketError), "Socket error occurred: " + std::to_string(static_cast<int>(socketError)))
  ));
}

void NetClientServerSession::handleReadyRead() {
  using utility::functions::fromQByteArray;
  m_ssl_socket->setProperty(READ_TIME, QDateTime::currentDateTime());

  // get the first four bytes of the packet
  QDataStream get(m_ssl_socket);

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
  if (m_ssl_socket->bytesAvailable() < toRead) {
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
    emit this->networkPacket(this, fromQByteArray<packets::Authentication>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket& e) {
    qDebug() << e.what();
    return;
  } catch (const common::types::exceptions::NotThisPacket& e) {
    // No-Op
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
    // No-Op
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
    // No-Op
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
    // No-Op
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

NetClientServerSession::NetClientServerSession(
  common::trust::TrustedServers* trustedServers,
  const NetResolvedDevice& device,
  const common::types::SslConfig& sslConfig,
  QObject* parent
) : Session(device.name, parent),
    trustedServers(trustedServers),
    device(device),
    sslConfig(sslConfig) {
  QObject::connect(
    m_pingTimer, &QTimer::timeout,
    this, &NetClientServerSession::handlePingTimeout
  );

  QObject::connect(
    m_ssl_socket,
    &QSslSocket::connected,
    this,
    &NetClientServerSession::handleConnected
  );

  QObject::connect(
    m_ssl_socket,
    &QSslSocket::disconnected,
    this,
    &NetClientServerSession::handleDisconnected
  );

  QObject::connect(
    m_ssl_socket,
    &QSslSocket::errorOccurred,
    this,
    &NetClientServerSession::handleError
  );

  QObject::connect(
    m_ssl_socket,
    &QSslSocket::readyRead,
    this,
    &NetClientServerSession::handleReadyRead
  );

  QObject::connect(
    m_pongTimer, &QTimer::timeout,
    this, &NetClientServerSession::handlePongTimeout
  );

  QObject::connect(
    this->trustedServers,
    &common::trust::TrustedServers::trustedServersChanged,
    this,
    &NetClientServerSession::handleTrustedServersChanged
  );
}

NetClientServerSession::~NetClientServerSession() {
  // Nothing to do here
}

void NetClientServerSession::sendPacket(const packets::NetworkPacket& packet) {
  this->m_ssl_socket->write(packet.toBytes());
}

void NetClientServerSession::disconnectFromHost() {
  this->m_ssl_socket->disconnectFromHost();
}

bool NetClientServerSession::isTrusted() const {
  return this->trustedServers->isTrustedServer(this->device.name, m_ssl_socket->peerCertificate().toPem());
}

QByteArray NetClientServerSession::getCertificate() const {
  return m_ssl_socket->peerCertificate().toPem();
}

void NetClientServerSession::connect() {
  if (this->m_ssl_socket->state() == QAbstractSocket::ConnectedState) {
    this->m_ssl_socket->abort();
  }

  QSslConfiguration ssl = m_ssl_socket->sslConfiguration();
  ssl.setPrivateKey(QSslKey(sslConfig.privateKey, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey));
  ssl.setLocalCertificate(QSslCertificate(sslConfig.certificate, QSsl::Pem));
  m_ssl_socket->setSslConfiguration(ssl);

  QObject::connect(
    m_ssl_socket,
    &QSslSocket::sslErrors,
    this,
    &NetClientServerSession::handleSslErrors
  );

  m_ssl_socket->connectToHostEncrypted(device.host.toString(), device.port);
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing::network
