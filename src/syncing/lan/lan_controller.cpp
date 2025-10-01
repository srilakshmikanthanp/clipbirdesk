#include "lan_controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::syncing::lan {
LanController::LanController(QSslConfiguration config, QObject *parent) : controller::Controller(parent), m_sslConfig(config), m_host(std::in_place_type<Client>, constants::getMDnsServiceName(), constants::getMDnsServiceType(), this) {}
LanController::~LanController() = default;

void LanController::synchronize(const QVector<QPair<QString, QByteArray>> &data) {
  if (std::holds_alternative<Client>(m_host)) {
    if (std::get<Client>(m_host).isConnectedToServer()) std::get<Client>(m_host).synchronize(data);
  } else if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).synchronize(data);
  } else {
    throw std::runtime_error("Host is neither server nor client");
  }
}

QSslConfiguration LanController::getSslConfiguration() const {
  return m_sslConfig;
}

void LanController::setSslConfiguration(const QSslConfiguration &config) {
  m_sslConfig = config;
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).setSslConfiguration(config);
  } else if (std::holds_alternative<Client>(m_host)) {
    std::get<Client>(m_host).setSslConfiguration(config);
  } else {
    throw std::runtime_error("Host is neither server nor client");
  }
}

std::variant<syncing::lan::Server, syncing::lan::Client>& LanController::getHost() {
  return m_host;
}

types::enums::HostType LanController::getHostType() const {
  if (std::holds_alternative<Server>(m_host)) {
    return types::enums::HostType::SERVER;
  } else if (std::holds_alternative<Client>(m_host)) {
    return types::enums::HostType::CLIENT;
  } else {
    return types::enums::HostType::NONE;
  }
}

void LanController::setHostAsServer() {
  auto *server = &m_host.emplace<Server>(constants::getMDnsServiceName(), constants::getMDnsServiceType(), this);
  server->setSslConfiguration(m_sslConfig);

  connect(
    server, &Server::OnMdnsRegisterStatusChangeChanged,
    this, &LanController::OnMdnsRegisterStatusChangeChanged
  );

  connect(
    server, &Server::OnCLientStateChanged,
    this, &LanController::OnCLientStateChanged
  );

  connect(
    server, &Server::OnSyncRequest,
    this, &LanController::OnSyncRequest
  );

  connect(
    server, &Server::OnClientListChanged,
    this, &LanController::OnClientListChanged
  );

  connect(
    server, &Server::OnAuthRequest,
    this, &LanController::OnAuthRequest
  );

  server->startServer();

  emit OnHostTypeChanged(types::enums::HostType::SERVER);
}

void LanController::setHostAsClient() {
  auto *client = &m_host.emplace<Client>(constants::getMDnsServiceName(), constants::getMDnsServiceType(), this);

  client->setSslConfiguration(m_sslConfig);

  connect(
    client, &Client::OnServerListChanged,
    this, &LanController::OnServerListChanged
  );

  connect(
    client, &Client::OnServerFound,
    this, &LanController::OnServerFound
  );

  connect(
    client, &Client::OnServerGone,
    this, &LanController::OnServerGone
  );

  connect(
    client, &Client::OnServerStatusChanged,
    this, &LanController::OnServerStatusChanged
  );

  connect(
    client, &Client::OnSyncRequest,
    this, &LanController::OnSyncRequest
  );

  connect(
    client, &Client::OnConnectionError,
    this, &LanController::OnConnectionError
  );

  client->getMdnsBrowser()->startBrowsing();

  emit OnHostTypeChanged(types::enums::HostType::CLIENT);
}

void LanController::authSuccess(const types::Device &client) {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).authSuccess(client);
  } else {
    throw std::runtime_error("Host is not server");
  }
}

void LanController::authFailed(const types::Device &client) {
  if (std::holds_alternative<Server>(m_host)) {
    std::get<Server>(m_host).authFailed(client);
  } else {
    throw std::runtime_error("Host is not server");
  }
}

void LanController::disposeServer() {
  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }
  auto *server = &std::get<Server>(m_host);
  server->disconnectAllClients();
  server->stopServer();
}

void LanController::disposeClient() {
  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }
  auto *client = &std::get<Client>(m_host);
  client->disconnectFromServer();
}
}
