#include "bt_server.hpp"

#include <QSslCertificate>
#include <QSslKey>

#include "bt_constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::bluetooth {
void BtServer::handleCertificateExchangePacket(const packets::CertificateExchangePacket& packet) {
  auto client = qobject_cast<QBluetoothSocket *>(sender());
  auto name = client->peerName();
  auto session = new BtServerClientSession(name, packet.getCertificate(), trustedClients, client, this);
  client->setProperty(SESSION, QVariant::fromValue<QObject*>(session));
  client->setParent(session);
  m_clients.append(session);
  emit onClientConnected(session);
}

void BtServer::handlePendingConnections() {
  while (m_server->hasPendingConnections()) {
    auto client = qobject_cast<QBluetoothSocket *>(m_server->nextPendingConnection());

    QObject::connect(
      client,
      &QBluetoothSocket::disconnected,
      this,
      &BtServer::handleClientDisconnection
    );

    QObject::connect(
      client,
      &QBluetoothSocket::readyRead,
      this,
      &BtServer::handleClientReadyRead
    );

    client->write(utility::functions::createPacket({this->sslConfig.certificate}).toBytes());
  }
}

void BtServer::handleError(QAbstractSocket::SocketError socketError) {
  QBluetoothSocket* socket = qobject_cast<QBluetoothSocket *>(sender());
  BtServerClientSession* session = socket->property(SESSION).value<BtServerClientSession*>();

  if (session == nullptr) {
    return;
  }

  emit onClientError(session, std::make_exception_ptr(
    common::types::exceptions::ErrorCodeException(static_cast<int>(socketError), "Socket error occurred: " + std::to_string(static_cast<int>(socketError)))
  ));
}

void BtServer::handleClientDisconnection() {
  auto client = qobject_cast<QBluetoothSocket *>(sender());
  QList<BtServerClientSession*>::iterator iterator = std::find_if(m_clients.begin(), m_clients.end(), [&](BtServerClientSession* c) {
    return c->getSocket() == client;
  });

  if (iterator == m_clients.end()) {
    return;
  }

  m_clients.erase(iterator);
  emit onClientDisconnected(*iterator);
}

void BtServer::handleClientReadyRead() {
  auto client = qobject_cast<QBluetoothSocket *>(sender());
  client->setProperty(READ_TIME, QDateTime::currentDateTime());

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

  try {
    emit this->handleCertificateExchangePacket(fromQByteArray<packets::CertificateExchangePacket>(data));
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

  if (!client->property(SESSION).isValid()) {
    return;
  }

  BtServerClientSession* session = client->property(SESSION).value<BtServerClientSession*>();

  if (session == nullptr) {
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


void BtServer::handlePingTimeout() {
  using utility::functions::params::PingPacketParams;
  using packets::PingPongPacket;
  using utility::functions::createPacket;
  auto pingPacket = createPacket(PingPacketParams{
    common::types::enums::PingType::Ping
  });
}

void BtServer::handlePongTimeout() {
  for (auto client : m_clients) {
    auto lastRead = client->property(READ_TIME).toDateTime();
    auto now      = QDateTime::currentDateTime();
    auto diff     = lastRead.msecsTo(now);

    if (diff > constants::getAppMaxReadIdleTime()) {
      client->disconnectFromHost();
    }
  }
}

BtServer::BtServer(const common::types::SslConfig sslConfig, common::trust::TrustedClients* trustedClients, QObject *parent): Server(sslConfig, parent), trustedClients(trustedClients) {
  m_server->setSecurityFlags(QBluetooth::Security::Encryption | QBluetooth::Security::Secure);

  QObject::connect(
    m_server, &QBluetoothServer::newConnection,
    this, &BtServer::handlePendingConnections
  );
  connect(
    this->m_pingTimer, &QTimer::timeout,
    this, &BtServer::handlePingTimeout
  );
  connect(
    this->m_pongTimer, &QTimer::timeout,
    this, &BtServer::handlePongTimeout
  );

  this->m_pingTimer->start(constants::getAppMaxWriteIdleTime());
  this->m_pongTimer->start(constants::getAppMaxReadIdleTime());
}

BtServer::~BtServer() {
  this->stop();
}

void BtServer::start() {
  serviceInfo = m_server->listen(QBluetoothUuid(BT_SERVICE_UUID), tr(constants::getMDnsServiceName().toStdString().c_str()));
}

void BtServer::stop() {
  if (serviceInfo.isRegistered()) {
    serviceInfo.unregisterService();
  }

  if (m_server->isListening()) {
    m_server->close();
  }

  if (m_pingTimer->isActive()) {
    m_pingTimer->stop();
  }

  if (m_pongTimer->isActive()) {
    m_pongTimer->stop();
  }

  for (auto client : m_clients) {
    client->disconnectFromHost();
  }

  m_clients.clear();
}
}  // namespace srilakshmikanthanp::clipbirdesk::syncing
