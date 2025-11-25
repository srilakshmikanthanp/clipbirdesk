#include "net_server.hpp"

#include <QSslCertificate>
#include <QSslKey>

namespace srilakshmikanthanp::clipbirdesk::syncing::network {
void NetServer::handlePendingConnections() {
  while (m_server->hasPendingConnections()) {
    auto client = qobject_cast<QSslSocket *>(m_server->nextPendingConnection());
    auto cert = client->peerCertificate();
    auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

    QObject::connect(
      client,
      &QSslSocket::disconnected,
      this,
      &NetServer::handleClientDisconnection
    );

    QObject::connect(
      client,
      &QSslSocket::readyRead,
      this,
      &NetServer::handleClientReadyRead
    );

    auto session = new NetServerClientSession(name, cert.toPem(), trustedClients, client, this);
    client->setParent(session);
    client->setProperty(SESSION, QVariant::fromValue<QObject*>(session));
    m_clients.append(session);
    emit onClientConnected(session);
  }
}

void NetServer::handleSslErrors(QSslSocket *socket, const QList<QSslError>& errors) {
  QList<QSslError::SslError> ignoredErrors;
  ignoredErrors.append(QSslError::SelfSignedCertificate);
  ignoredErrors.append(QSslError::UnspecifiedError);
  auto errorsCopy = errors;

  // remove all the ignored errors
  auto itr = std::remove_if(errorsCopy.begin(), errorsCopy.end(), [&](auto error) {
    return ignoredErrors.contains(error.error());
  });

  errorsCopy.erase(itr, errorsCopy.end());

  // if errorsCopy is not empty
  if (!errorsCopy.isEmpty()) {
    return socket->abort();
  }

  auto cert = socket->peerCertificate();
  auto name = cert.subjectInfo(QSslCertificate::CommonName);

  // if certificate is null or common name is empty
  if (cert.isNull() || name.isEmpty()) {
    return socket->abort();
  }

  // just ignore wait for user
  socket->ignoreSslErrors();
}

void NetServer::handleError(QAbstractSocket::SocketError socketError) {
  QSslSocket* socket = qobject_cast<QSslSocket *>(sender());
  NetServerClientSession* session = socket->property(SESSION).value<NetServerClientSession*>();

  if (session == nullptr) {
    return;
  }

  emit onClientError(session, std::make_exception_ptr(
    common::types::exceptions::ErrorCodeException(static_cast<int>(socketError), "Socket error occurred: " + std::to_string(static_cast<int>(socketError)))
  ));
}

void NetServer::handleClientDisconnection() {
  auto client = qobject_cast<QSslSocket *>(sender());
  QList<NetServerClientSession*>::iterator iterator = std::find_if(m_clients.begin(), m_clients.end(), [&](NetServerClientSession* c) {
    return c->getSocket() == client;
  });

  if (iterator == m_clients.end()) {
    return;
  }

  m_clients.erase(iterator);
  emit onClientDisconnected(*iterator);
}

void NetServer::handleClientReadyRead() {
  auto client = qobject_cast<QSslSocket *>(sender());
  client->setProperty(READ_TIME, QDateTime::currentDateTime());
  NetServerClientSession* session = client->property(SESSION).value<NetServerClientSession*>();

  if (session == nullptr) {
    return;
  }

  using utility::functions::createPacket;
  using utility::functions::fromQByteArray;

  // get the first four bytes of the packet
  QDataStream get(client);

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
  qint32 toRead = packetLength - data.size();

  // check has enough bytes
  if (client->bytesAvailable() < toRead) {
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

  // Deserialize the data to SyncingPacket
  try {
    emit this->onNetworkPacket(session, fromQByteArray<packets::SyncingPacket>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket &e) {
    session->sendPacket(createPacket({e.getCode(), e.what()}));
    return;
  } catch (const common::types::exceptions::NotThisPacket &e) {
    qDebug() << e.what();
  } catch (const std::exception &e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // Deserialize the data to SyncingPacket
  try {
    emit this->onNetworkPacket(session, fromQByteArray<packets::PingPongPacket>(data));
    return;
  } catch (const common::types::exceptions::MalformedPacket &e) {
    session->sendPacket(createPacket({e.getCode(), e.what()}));
    return;
  } catch (const common::types::exceptions::NotThisPacket &e) {
    qDebug() << e.what();
  } catch (const std::exception &e) {
    qDebug() << e.what();
    return;
  } catch (...) {
    qDebug() << "Unknown Error";
    return;
  }

  // if the packet is none of the above then send the invalid packet
  const auto code = common::types::enums::ErrorCode::InvalidPacket;
  const auto msg  = "Invalid Packet";
  session->sendPacket(createPacket({code, msg}));
}


void NetServer::handlePingTimeout() {
  using utility::functions::params::PingPacketParams;
  using packets::PingPongPacket;
  using utility::functions::createPacket;
  auto pingPacket = createPacket(PingPacketParams{
    common::types::enums::PingType::Ping
  });
}

void NetServer::handlePongTimeout() {
  for (auto client : m_clients) {
    auto lastRead = client->property(READ_TIME).toDateTime();
    auto now      = QDateTime::currentDateTime();
    auto diff     = lastRead.msecsTo(now);

    if (diff > constants::getAppMaxReadIdleTime()) {
      client->disconnect();
    }
  }
}

NetServer::NetServer(const common::types::SslConfig sslConfig, common::trust::TrustedClients* trustedClients, QObject *parent): Server(sslConfig, parent), trustedClients(trustedClients) {
  connect(
    this->m_mdnsRegister, &MdnsRegister::OnServiceUnregisteringFailed,
    this, &NetServer::onServiceUnregistrationFailed
  );
  connect(
    this->m_mdnsRegister, &MdnsRegister::OnServiceRegisteringFailed,
    this, &NetServer::onServiceRegistrationFailed
  );
  connect(
    this->m_mdnsRegister, &MdnsRegister::OnServiceRegistered,
    this, &NetServer::onServiceRegistered
  );
  connect(
    this->m_mdnsRegister, &MdnsRegister::OnServiceUnregistered,
    this, &NetServer::onServiceUnregistered
  );
  connect(
    this->m_pingTimer, &QTimer::timeout,
    this, &NetServer::handlePingTimeout
  );
  connect(
    this->m_pongTimer, &QTimer::timeout,
    this, &NetServer::handlePongTimeout
  );
  QObject::connect(
    m_server, &QSslServer::sslErrors,
    this, &NetServer::handleSslErrors
  );
  QObject::connect(
    m_server, &QSslServer::pendingConnectionAvailable,
    this, &NetServer::handlePendingConnections
  );

  this->m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  this->m_pongTimer->start(constants::getAppMaxReadIdleTime());
}

NetServer::~NetServer() {
  this->stop();
}

void NetServer::start() {
  QSslConfiguration ssl = m_server->sslConfiguration();
  ssl.setPrivateKey(QSslKey(sslConfig.privateKey, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey));
  ssl.setLocalCertificate(QSslCertificate(sslConfig.certificate, QSsl::Pem));
  m_server->setSslConfiguration(ssl);

  if (!m_server->listen()) {
    throw std::runtime_error("Failed to start the server");
  }

  auto port = std::to_string(m_server->serverPort());
  m_mdnsRegister->registerService(m_server->serverPort());
  m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  m_pongTimer->start(constants::getAppMaxReadIdleTime());
}

void NetServer::stop() {
  m_mdnsRegister->unregisterService();
  m_server->close();
  m_pingTimer->stop();
  m_pongTimer->stop();

  for (auto client : m_clients) {
    client->disconnect();
  }

  m_clients.clear();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
