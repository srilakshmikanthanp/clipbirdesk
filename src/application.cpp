// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "application.hpp"

namespace srilakshmikanthanp::clipbirdesk {

QSslConfiguration Application::getOldSslConfiguration() {
  auto two_months      = std::chrono::milliseconds(constants::getAppCertExpiryInterval());
  auto& storage        = storage::Storage::instance();
  QSslCertificate cert = storage.getHostCert();
  QSslKey key          = storage.getHostKey();
  QSslConfiguration sslConfig;

  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  if (name != constants::getMDnsServiceName()) {
    return getNewSslConfiguration();
  }

  sslConfig.setLocalCertificate(cert);
  sslConfig.setPrivateKey(key);

  if (cert.isNull() || key.isNull() || sslConfig.isNull()) {
    throw std::runtime_error("Can't Create QSslConfiguration");
  }

  if (cert.expiryDate() - QDateTime::currentDateTime() < two_months) {
    return getNewSslConfiguration();
  }

  sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
  return sslConfig;
}

QSslConfiguration Application::getNewSslConfiguration() {
  auto sslConfig = utility::functions::getQSslConfiguration();
  auto& storage  = storage::Storage::instance();
  storage.setHostCert(sslConfig.localCertificate());
  storage.setHostKey(sslConfig.privateKey());
  return sslConfig;
}

QSslConfiguration Application::getSslConfiguration() {
  auto& storage = storage::Storage::instance();
  QSslConfiguration config;

  if (!storage.hasHostCert() || !storage.hasHostKey()) {
    config = getNewSslConfiguration();
  } else {
    config = getOldSslConfiguration();
  }

  return config;
}

QFuture<void> Application::setupHubConnection() {
  const auto updateHubHostDevice = [=, this](const syncing::wan::HubHostDevice& device) {
    auto requestDto = syncing::wan::DeviceRequestDto{ device.publicKey, device.name, device.type };
    return deviceRepository->updateDevice(device.id, requestDto).then([=, this](const syncing::wan::DeviceResponseDto& updatedDevice) {
      return syncing::wan::HubHostDevice{updatedDevice.id, updatedDevice.name, updatedDevice.type, updatedDevice.publicKey, device.privateKey};
    });
  };

  const auto createHubHostDevice = [=, this]() {
    auto [privateKey, publicKey] = utility::functions::generateQtKeyPair();
    auto requestDto = syncing::wan::DeviceRequestDto{ publicKey.toPem(), constants::getMDnsServiceName(), syncing::wan::DeviceType::getCurrentDeviceType() };
    return deviceRepository->createDevice(requestDto).then([=, this](const syncing::wan::DeviceResponseDto& device) {
      return syncing::wan::HubHostDevice{device.id, device.name, device.type, device.publicKey, privateKey.toPem()};
    });
  };

  const auto saveHubHostDevice = [=](const syncing::wan::HubHostDevice& device) {
    QKeychain::WritePasswordJob *job = nullptr;
    bool status = QMetaObject::invokeMethod(
      &storage::SecureStorage::instance(),
      &storage::SecureStorage::setHubHostDevice,
      Qt::BlockingQueuedConnection,
      qReturnArg(job),
      device
    );
    assert(status && job != nullptr);
    return utility::functions::toFuture(job).then([device]() {
      QPromise<syncing::wan::HubHostDevice> promise;
      promise.addResult(device);
      promise.finish();
      return promise.future();
    }).unwrap();
  };

  this->trayMenu->setHubEnabled(false);

  return utility::functions::toFuture(
    storage::SecureStorage::instance().getHubHostDevice(),
    &storage::ReadHubHostDeviceJob::device
  ).then([=, this](std::optional<syncing::wan::HubHostDevice> device) {
    if (device.has_value()) {
      syncing::wan::HubHostDevice hubDevice = device.value();
      hubDevice.name = constants::getMDnsServiceName();
      return updateHubHostDevice(hubDevice);
    } else {
      return createHubHostDevice();
    }
  })
  .unwrap()
  .then([=, this](const syncing::wan::HubHostDevice& device) {
    return saveHubHostDevice(device);
  })
  .unwrap()
  .then([=, this](const syncing::wan::HubHostDevice& device) {
    wanController->connectToHub(device);
  })
  .onFailed([=, this](const std::exception& e) {
    this->trayMenu->setHubEnabled(true);
    this->trayIcon->showMessage(
      constants::getAppName(),
      QObject::tr("Hub Connection Error Occurred: %1").arg(e.what()),
      QIcon(QString::fromStdString(constants::getAppLogo()))
    );
  });
}

void Application::handleConnectionError(QString error) {
  auto message = QObject::tr("Connection Error: %1").arg(error);
  auto title = constants::getAppName();
  auto icon = QIcon(QString::fromStdString(constants::getAppLogo()));
  trayIcon->showMessage(title, message, icon);
}

void Application::handleTabChange(ui::gui::widgets::Clipbird::Tabs tab) {
  if (tab == ui::gui::widgets::Clipbird::Tabs::Client) {
    storage::Storage::instance().setHostIsServer(false);
    this->trayMenu->setQrCodeEnabled(false);
    this->trayMenu->setConnectEnabled(true);
    lanController->setHostAsClient();
  } else {
    storage::Storage::instance().setHostIsServer(true);
    this->trayMenu->setQrCodeEnabled(true);
    this->trayMenu->setConnectEnabled(false);
    lanController->setHostAsServer();
  }
}

void Application::handleAuthRequest(const types::Device& client) {
  auto &m_host = lanController->getHost();

  if (!std::holds_alternative<Server>(m_host)) {
    throw std::runtime_error("Host is not server");
  }

  auto cert = std::get<Server>(m_host).getUnauthedClientCert(client);
  auto &store = storage::Storage::instance();
  if (store.hasClientCert(client.name)) {
    if (store.getClientCert(client.name) == cert.toPem()) {
      return lanController->authSuccess(client);
    }
  }

  ui::gui::notification::JoinRequest* toast = new ui::gui::notification::JoinRequest(this);

  const auto connectionAccept = connect(
    toast, &ui::gui::notification::JoinRequest::onAccept,
    [=, this] { lanController->authSuccess(client); }
  );

  connect(
    toast, &ui::gui::notification::JoinRequest::onAccept,
    toast, &QObject::deleteLater
  );

  connect(clipbird, &ui::gui::widgets::Clipbird::onTabChanged, [=, this]{
    QObject::disconnect(connectionAccept);
  });

  const auto connectionReject = connect(
    toast, &ui::gui::notification::JoinRequest::onReject,
    [=, this] { lanController->authFailed(client); }
  );

  connect(
    toast, &ui::gui::notification::JoinRequest::onReject,
    toast, &QObject::deleteLater
  );

  connect(clipbird, &ui::gui::widgets::Clipbird::onTabChanged, [=, this]{
    QObject::disconnect(connectionReject);
  });

  toast->show(client);
}


void Application::handleConnect(QString ip, QString port) {
  const auto slot_hr = [=, this](QWidget* dialog, quint16 port, const auto& host) {
    if (host.error() != QHostInfo::NoError) {
      return;
    }
    dialog->setVisible(false);
    std::get<Client>(lanController->getHost()).connectToServer(
      types::Device{host.addresses().first(), port, host.hostName()}
    );
  };

  const auto validator = [](auto ip, auto port) -> bool {
    if (!QHostAddress(ip).isNull() && port > 0 && port < 65535) {
      return true;
    } else {
      return false;
    }
  };

  if (!validator(ip.toShort(), port.toShort())) {
    return;
  }

  auto slot = std::bind(
    slot_hr, joiner, port.toShort(), std::placeholders::_1
  );

  QHostInfo::lookupHost(ip, this, slot);
}

void Application::handleSignin(QString email, QString password) {
  trayMenu->setAccoundEnabled(false);
  signin->setSigningIn(true);
  authRepository->signIn({email, password})
  .then([=, this](const syncing::wan::AuthTokenDto& token) {
    QKeychain::WritePasswordJob *job = nullptr;
    bool status = QMetaObject::invokeMethod(
      &storage::SecureStorage::instance(),
      &storage::SecureStorage::setHubAuthTokenToken,
      Qt::BlockingQueuedConnection,
      qReturnArg(job),
      token
    );
    assert(status && job != nullptr);
    return utility::functions::toFuture(job).then([token, this]() {
      syncing::wan::AuthTokenHolder::instance().setAuthToken(token);
      this->signin->setError(std::nullopt);
      this->signin->reset();
      this->signin->hide();
      this->trayMenu->setAccoundEnabled(true);
      this->signin->setSigningIn(false);
    });
  }).unwrap().onFailed([=, this](const std::exception& e) {
    this->signin->setError(e.what());
    this->trayMenu->setAccoundEnabled(true);
    this->signin->setSigningIn(false);
  });
}

void Application::handleHubConnect() {
  this->trayMenu->setJoinedToHub(true);
  this->trayMenu->setHubEnabled(true);
}

void Application::handleHubDisconnect() {
  this->trayMenu->setJoinedToHub(false);
  this->trayMenu->setHubEnabled(true);
}

void Application::handleHubErrorOccurred(QAbstractSocket::SocketError error) {
  this->trayIcon->showMessage(
    constants::getAppName(),
    QObject::tr("Hub Connection Error Occurred: %1").arg(QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey(error)),
    QIcon(QString::fromStdString(constants::getAppLogo()))
  );
}

void Application::handleSleepEvent() {
  switch (lanController->getHostType()) {
  case types::enums::HostType::CLIENT:
    lanController->disposeClient();
    break;

  case types::enums::HostType::SERVER:
    lanController->disposeServer();
    break;
  }
}

void Application::handleWakeUpEvent() {
  if (storage::Storage::instance().getHostIsServer()) {
    lanController->setHostAsServer();
  } else {
    lanController->setHostAsClient();
  }
}

void Application::handleClientStateChanged(types::Device client, bool connected) {
  auto &host = lanController->getHost();

  if (!std::holds_alternative<Server>(host)) {
    throw std::runtime_error("Host is not server");
  }

  if (!connected) return;

  auto cert = std::get<Server>(host).getClientCert(client);
  auto &store = storage::Storage::instance();
  store.setClientCert(client.name, cert.toPem());
  auto sslConfig = lanController->getSslConfiguration();
  sslConfig.addCaCertificate(cert);
  lanController->setSslConfiguration(sslConfig);
}

void Application::handleServerStatusChanged(bool status, types::Device host) {
  auto &m_host = lanController->getHost();

  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  if (status) {
    auto cert = client->getConnectedServerCertificate();
    auto name = host.name;
    store.setServerCert(name, cert.toPem());
    auto m_sslConfig = client->getSslConfiguration();
    m_sslConfig.addCaCertificate(cert);
    client->setSslConfiguration(m_sslConfig);
    return;
  }

  for (auto s : client->getServerList()) {
    if (host != s && store.hasServerCert(s.name)) {
      return client->connectToServerSecured(s);
    }
  }
}

void Application::handleServerFound(types::Device server) {
  auto &m_host = lanController->getHost();

  if (!std::holds_alternative<Client>(m_host)) {
    throw std::runtime_error("Host is not client");
  }

  auto connectedServer = std::get<Client>(m_host).getConnectedServer();
  if (connectedServer.has_value()) return;
  auto *client = &std::get<Client>(m_host);
  auto &store  = storage::Storage::instance();

  if (store.hasServerCert(server.name)) {
    client->connectToServerSecured(server);
  }
}

void Application::handleOnClipboard(QVector<QPair<QString, QByteArray>> data) {
  clipboardController->getClipboard().set(data);
}

void Application::handleSyncRequest(QVector<QPair<QString, QByteArray>> data) {
  historyController->addHistory(data);
}

void Application::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
    clipbird->isVisible() ? clipbird->hide() : this->openClipbird();
  }
}

void Application::openClipbird() {
  this->clipbird->setFixedSize(constants::getAppWindowSize());
  this->clipbird->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));
  this->clipbird->setWindowTitle(constants::getAppName());
  this->clipbird->show();
}

void Application::openQrCode() {
  if (group->isVisible()) { return group->raise(); }
  auto server = std::get<Server>(lanController->getHost()).getServerInfo();
  auto interfaces = QNetworkInterface::allInterfaces();
  const auto addrs = QNetworkInterface::allAddresses();

  // construct json object { "port": 1234, "ips": [...]  }
  QJsonObject json;
  json.insert("port", server.port);
  QJsonArray ips;

  for (auto addr: addrs) {
    if (!(addr.protocol() == QAbstractSocket::IPv6Protocol || addr.isLoopback() || addr.toString().startsWith("127."))) {
      ips.append(addr.toString());
    }
  }

  json.insert("ips", ips);

  auto info = QJsonDocument(json).toJson(QJsonDocument::Compact);
  group->setWindowIcon(QIcon(constants::getAppLogo()));
  group->setPort(QString::number(server.port));
  group->setWindowTitle(constants::getAppName());
  group->setFixedSize(group->sizeHint());
  group->setQrCode(QString(info));
  group->show();
  group->setFixedSize(group->sizeHint());
  group->move(QGuiApplication::primaryScreen()->availableGeometry().center() - group->rect().center());
}

void Application::openConnect() {
  if (joiner->isVisible()) { return joiner->raise(); }
  joiner->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));
  joiner->setWindowTitle(constants::getAppName());
  joiner->setFixedSize(joiner->sizeHint());
  joiner->show();
  joiner->move(QGuiApplication::primaryScreen()->availableGeometry().center() - joiner->rect().center());
}

void Application::openAbout() {
  if (aboutUs->isVisible()) { return aboutUs->raise(); }
  aboutUs->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));
  aboutUs->setWindowTitle(constants::getAppName());
  aboutUs->setFixedSize(aboutUs->sizeHint());
  aboutUs->show();
  aboutUs->move(QGuiApplication::primaryScreen()->availableGeometry().center() - aboutUs->rect().center());
}

void Application::sendClipboard() {
  Q_UNUSED(QtConcurrent::run([this]() {
    auto content = clipboardController->getClipboard().get();
    if (content.isEmpty()) return;
    QTimer::singleShot(0, lanController, [=, this]() {
      lanController->synchronize(content);
      wanController->synchronize(content);
    });
  }));
}

void Application::openHistory() {
  if (history->isVisible()) { return history->raise(); }
  history->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));
  history->setWindowTitle(constants::getAppName());
  history->setHistory(historyController->getHistory());
  history->setFixedSize(constants::getAppWindowSize());
  history->show();
}

void Application::resetDevices() {
  storage::Storage::instance().clearAllClientCert();
  storage::Storage::instance().clearAllServerCert();
}

void Application::onAccountClicked() {
  if (this->trayMenu->isSignedIn()) {
    utility::functions::toFuture(storage::SecureStorage::instance().removeHubHostDevice()).then([=, this]() {
      return utility::functions::toFuture(storage::SecureStorage::instance().removeHubJwtToken());
    }).unwrap().then([=, this]() {
      syncing::wan::AuthTokenHolder::instance().setAuthToken(std::nullopt);
    }).onFailed([=, this](const std::exception& e) {
      this->trayIcon->showMessage(constants::getAppName(), QObject::tr("Error Signing Out: %1").arg(e.what()), QIcon(QString::fromStdString(constants::getAppLogo())));
    });
    return;
  }

  if (signin->isVisible()) { return signin->raise(); }
  signin->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));
  signin->setWindowTitle(constants::getAppName());
  signin->setFixedSize(signin->sizeHint());
  signin->show();
}

void Application::onHubClicked() {
  if (this->trayMenu->isJoinedToHub()) {
    storage::Storage::instance().setIsUserConnectedToHubLastly(false);
    this->trayMenu->setHubEnabled(false);
    wanController->disconnectFromHub();
  } else {
    storage::Storage::instance().setIsUserConnectedToHubLastly(true);
    this->setupHubConnection();
  }
}

void Application::setQssFile(Qt::ColorScheme scheme) {
  bool isDark     = scheme == Qt::ColorScheme::Dark;
  std::string qss = isDark ? constants::getAppQSSDark() : constants::getAppQSSLight();

  QFile qssFile(QString::fromStdString(qss));
  qssFile.open(QFile::ReadOnly);
  qApp->setStyleSheet(qssFile.readAll());
}

Application::Application(int &argc, char **argv) : SingleApplication(argc, argv) {
  clipboardController = new controller::ClipboardController(this);
  historyController   = new controller::HistoryController(this);
  lanController       = new controller::LanController(getSslConfiguration(), this);
  wanController       = new controller::WanController(this);
  hotkey              = new QHotkey(QKeySequence(constants::getAppHistoryShortcut()), true, this);
  clipbird            = new ui::gui::widgets::Clipbird();
  history             = new ui::gui::widgets::History();
  trayMenu            = new ui::gui::TrayMenu();
  trayIcon            = new QSystemTrayIcon();
  powerHandler        = new PowerHandler(this);

  utility::functions::toFuture(
    storage::SecureStorage::instance().getHubAuthToken(),
    &storage::ReadAuthTokenDtoJob::authToken
  ).then([=, this](std::optional<syncing::wan::AuthTokenDto> token) {
    if (token.has_value()) syncing::wan::AuthTokenHolder::instance().setAuthToken(token.value());
    if (token.has_value() && storage::Storage::instance().getHubIsConnectedLastly()) setupHubConnection();
  }).onFailed([=, this](const std::exception& e) {
    throw std::runtime_error("Error reading Hub JWT Token: " + std::string(e.what()));
  });

  trayIcon->setIcon(QIcon(constants::getAppLogo()));
  trayIcon->setContextMenu(trayMenu);
  trayIcon->setToolTip(constants::getAppName());
  trayIcon->setToolTip(QString::fromStdString(constants::getAppName()));

  signal(SIGTERM, [](int sig) { qApp->quit(); });
  signal(SIGINT, [](int sig) { qApp->quit(); });
  signal(SIGABRT, [](int sig) { qApp->quit(); });

  setQssFile(QGuiApplication::styleHints()->colorScheme());

  qApp->setQuitOnLastWindowClosed(false);

  QObject::connect(
    trayIcon, &QSystemTrayIcon::activated,
    this, &Application::onTrayIconClicked
  );

  QObject::connect(
    this, &SingleApplication::instanceStarted,
    clipbird, &ui::gui::widgets::Clipbird::show
  );

  QObject::connect(
    QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
    this, &Application::setQssFile
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnQrCodeClicked,
    this, &Application::openQrCode
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnConnectClicked,
    this, &Application::openConnect
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnAboutClicked,
    this, &Application::openAbout
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnOpenAppClicked,
    this, &Application::openClipbird
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnHistoryClicked,
    this, &Application::openHistory
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnResetClicked,
    this, &Application::resetDevices
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnExitClicked,
    [this] { QApplication::quit(); }
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnAccountClicked,
    this, &Application::onAccountClicked
  );

  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnHubClicked,
    this, &Application::onHubClicked
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    this, &Application::handleTabChange
  );

  QObject::connect(
    history, &ui::gui::widgets::History::onClipSend,
    this, &Application::sendClipboard
  );

  QObject::connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    group, &QDialog::close
  );

  QObject::connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    joiner, &QDialog::close
  );

  connect(
    history, &ui::gui::widgets::History::onClipDelete,
    historyController, &controller::HistoryController::deleteHistoryAt
  );

  connect(
    historyController, &controller::HistoryController::OnHistoryChanged,
    history, &ui::gui::widgets::History::setHistory
  );

  connect(
    history, &ui::gui::widgets::History::onClipSelected,
    [=, this](auto i) { clipboardController->getClipboard().set(historyController->getHistory().at(i)); }
  );

  connect(
    lanController, &controller::LanController::OnClientListChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleClientListChange
  );

  connect(
    lanController, &controller::LanController::OnServerListChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleServerListChange
  );

  connect(
    lanController, &controller::LanController::OnMdnsRegisterStatusChangeChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleMdnsRegisterStatusChanged
  );

  connect(
    lanController, &controller::LanController::OnAuthRequest,
    this, &Application::handleAuthRequest
  );

  connect(
    lanController, &controller::LanController::OnServerStatusChanged,
    this, &Application::handleServerStatusChanged
  );

  connect(
    lanController, &controller::LanController::OnServerStatusChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleServerStatusChanged
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::disconnectFromServer,
    [=, this] { std::get<Client>(lanController->getHost()).disconnectFromServer(); }
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::disconnectClient,
    [=, this](auto client) { std::get<Server>(lanController->getHost()).disconnectClient(client); }
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::connectToServer,
    [=, this](auto server) { std::get<Client>(lanController->getHost()).connectToServer(server); }
  );

  connect(
    hotkey, &QHotkey::activated,
    this, &Application::openHistory
  );

  connect(
    &syncing::wan::AuthTokenHolder::instance(),
    &syncing::wan::AuthTokenHolder::authTokenChanged,
    [=, this](auto token) { this->trayMenu->setSignedIn(token.has_value()); }
  );

  connect(
    wanController, &controller::WanController::OnHubErrorOccurred,
    this, &Application::handleHubErrorOccurred
  );

  connect(
    wanController, &controller::WanController::OnHubConnected,
    this, &Application::handleHubConnect
  );

  connect(
    wanController, &controller::WanController::OnHubDisconnected,
    this, &Application::handleHubDisconnect
  );

  connect(
    joiner, &ui::gui::modals::Connect::onConnect,
    this, &Application::handleConnect
  );

  connect(
    signin, &ui::gui::modals::SignIn::onSignIn,
    this, &Application::handleSignin
  );

  connect(
    lanController, &controller::LanController::OnCLientStateChanged,
    this, &Application::handleClientStateChanged
  );

  connect(
    lanController, &controller::LanController::OnServerFound,
    this, &Application::handleServerFound
  );

  connect(
    lanController, &controller::LanController::OnConnectionError,
    this, &Application::handleConnectionError
  );

  connect(
    lanController, &controller::LanController::OnSyncRequest,
    this, &Application::handleSyncRequest
  );

  connect(
    wanController, &controller::WanController::OnSyncRequest,
    this, &Application::handleSyncRequest
  );

  connect(
    powerHandler, &PowerHandler::OnSleepEvent,
    this, &Application::handleSleepEvent
  );

  connect(
    powerHandler, &PowerHandler::OnWakeUpEvent,
    this, &Application::handleWakeUpEvent
  );

  connect(
    &clipboardController->getClipboard(),
    &clipboard::ApplicationClipboard::OnClipboardChange,
    lanController,
    &controller::LanController::synchronize
  );

  connect(
    &clipboardController->getClipboard(),
    &clipboard::ApplicationClipboard::OnClipboardChange,
    wanController,
    &controller::WanController::synchronize
  );

  connect(
    historyController,
    &controller::HistoryController::onClipboard,
    this,
    &Application::handleOnClipboard
  );

  if (storage::Storage::instance().getHostIsServer()) {
    clipbird->setTabAsServer();
  } else {
    clipbird->setTabAsClient();
  }

  trayIcon->show();
}

/**
 * @brief Destroy the Application object
 */
Application::~Application() {
  delete clipboardController;
  delete historyController;
  delete lanController;
  delete wanController;
  delete clipbird;
  delete history;
  delete trayMenu;
  delete trayIcon;
  delete aboutUs;
  delete group;
  delete joiner;
}

controller::ClipboardController* Application::getClipboardController() const {
  return clipboardController;
}

controller::HistoryController* Application::getHistoryController() const {
  return historyController;
}

controller::LanController* Application::getLanController() const {
  return lanController;
}

controller::WanController* Application::getWanController() const {
  return wanController;
}

PowerHandler* Application::getPowerHandler() const {
  return powerHandler;
}
}
