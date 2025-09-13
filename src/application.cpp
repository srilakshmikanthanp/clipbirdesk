// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "application.hpp"

namespace srilakshmikanthanp::clipbirdesk {

/**
 * @brief Update Hub Host Device
 */
QFuture<syncing::wan::HubHostDevice> Application::updateHubHostDevice(const syncing::wan::HubHostDevice& device) {
  auto requestDto = syncing::wan::DeviceRequestDto{ device.publicKey, device.name, device.type };
  return deviceRepository->updateDevice(device.id, requestDto).then([=, this](const syncing::wan::DeviceResponseDto& updatedDevice) {
    auto hubDevice = syncing::wan::HubHostDevice{updatedDevice.id, updatedDevice.name, updatedDevice.type, updatedDevice.publicKey, device.privateKey};
    return hubDevice;
  });
}

/**
 * @brief Create Hub Host Device
 */
QFuture<syncing::wan::HubHostDevice> Application::createHubHostDevice() {
  auto [privateKey, publicKey] = utility::functions::generateQtKeyPair();
  auto requestDto = syncing::wan::DeviceRequestDto{ publicKey.toPem(), constants::getMDnsServiceName(), syncing::wan::DeviceType::getCurrentDeviceType() };
  return deviceRepository->createDevice(requestDto).then([=, this](const syncing::wan::DeviceResponseDto& device) {
    auto hubDevice = syncing::wan::HubHostDevice{device.id, device.name, device.type, device.publicKey, privateKey.toPem()};
    return hubDevice;
  });
}

/**
 * @brief Save the Hub Host Device
 */
QFuture<syncing::wan::HubHostDevice> Application::saveHubHostDevice(const syncing::wan::HubHostDevice& device) {
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
}

/**
 * @brief Get the certificate from App Home
 */
QSslConfiguration Application::getOldSslConfiguration() {
  auto two_months      = std::chrono::milliseconds(constants::getAppCertExpiryInterval());
  auto& storage        = storage::Storage::instance();

  // read the certificate and key
  QSslCertificate cert = storage.getHostCert();
  QSslKey key          = storage.getHostKey();
  QSslConfiguration sslConfig;

  // get cert name
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // Name is updated
  if (name != constants::getMDnsServiceName()) {
    return getNewSslConfiguration();
  }

  // set the certificate and key
  sslConfig.setLocalCertificate(cert);
  sslConfig.setPrivateKey(key);

  // if the configuration is null
  if (cert.isNull() || key.isNull() || sslConfig.isNull()) {
    throw std::runtime_error("Can't Create QSslConfiguration");
  }

  // if the certificate is going to expiry
  if (cert.expiryDate() - QDateTime::currentDateTime() < two_months) {
    return getNewSslConfiguration();
  }

  // set peer verify
  sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);

  // return the configuration
  return sslConfig;
}

/**
 * @brief Get the certificate by creating new one
 */
QSslConfiguration Application::getNewSslConfiguration() {
  // generate the certificate and key
  auto sslConfig = utility::functions::getQSslConfiguration();

  // write the certificate and key
  auto& storage  = storage::Storage::instance();
  storage.setHostCert(sslConfig.localCertificate());
  storage.setHostKey(sslConfig.privateKey());

  // return the configuration
  return sslConfig;
}

/**
 * @brief Get the certificate from App Home
 * or generate new one and store it
 */
QSslConfiguration Application::getSslConfiguration() {
  auto& storage = storage::Storage::instance();
  QSslConfiguration config;

  if (!storage.hasHostCert() || !storage.hasHostKey()) {
    config = getNewSslConfiguration();
  } else {
    config = getOldSslConfiguration();
  }

  // return the configuration
  return config;
}

/**
 * @brief handle onConnectionError
 */
void Application::handleConnectionError(QString error) {
  // Just Show the error info to user via Dialog
  auto message = QObject::tr("Connection Error: %1").arg(error);

  // Title of the Notification
  auto title = constants::getAppName();

  // icon for the dialog
  auto icon = QIcon(QString::fromStdString(constants::getAppLogo()));

  // show notification
  trayIcon->showMessage(title, message, icon);
}

/**
 * @brief Connect to Hub
 */
void Application::connectToHub() {
  utility::functions::toFuture(
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
    controller->connectToHub(device);
  })
  .onFailed([=, this](const std::exception& e) {
    throw std::runtime_error("Error reading Hub Host Device: " + std::string(e.what()));
  });
}

/**
 * @brief On Tab Changed for Client
 */
void Application::handleTabChange(ui::gui::widgets::Clipbird::Tabs tab) {
  if (tab == ui::gui::widgets::Clipbird::Tabs::Client) {
    this->trayMenu->setQrCodeEnabled(false);
    this->trayMenu->setConnectEnabled(true);
    controller->setCurrentHostAsClient();
  } else {
    this->trayMenu->setQrCodeEnabled(true);
    this->trayMenu->setConnectEnabled(false);
    controller->setCurrentHostAsServer();
  }
}

/**
 * @brief On New Host Connected
 *
 * @param client
 */
void Application::handleAuthRequest(const types::Device& client) {
  // get the user input
  ui::gui::notification::JoinRequest* toast = new ui::gui::notification::JoinRequest(this);

  // connect the dialog to window AuthSuccess signal
  const auto connectionAccept = connect(
    toast, &ui::gui::notification::JoinRequest::onAccept,
    [=, this] { controller->authSuccess(client); }
  );

  connect(
    toast, &ui::gui::notification::JoinRequest::onAccept,
    toast, &QObject::deleteLater
  );

  // disconnect all signals on tab change signal
  connect(clipbird, &ui::gui::widgets::Clipbird::onTabChanged, [=, this]{
    QObject::disconnect(connectionAccept);
  });

  // connect the dialog to window AuthFail signal
  const auto connectionReject   = connect(
    toast, &ui::gui::notification::JoinRequest::onReject,
    [=, this] { controller->authFailed(client); }
  );

  connect(
    toast, &ui::gui::notification::JoinRequest::onReject,
    toast, &QObject::deleteLater
  );

  // disconnect all signals on tab change signal
  connect(clipbird, &ui::gui::widgets::Clipbird::onTabChanged, [=, this]{
    QObject::disconnect(connectionReject);
  });

  // shoe the notification
  toast->show(client);
}

/**
 * @brief handle signin
 */
void Application::handleSignin(QString email, QString password) {
  trayMenu->setAccoundEnabled(false);
  authRepository->signIn({email, password}).then([=, this](const syncing::wan::AuthTokenDto& token) {
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
    });
  }).unwrap().onFailed([=, this](const std::exception& e) {
      this->signin->setError(e.what());
      this->trayMenu->setAccoundEnabled(true);
  });
}

/**
 * @brief handle hub connect
 */
void Application::handleHubConnect() {
  this->trayMenu->setJoinedToHub(true);
}

/**
 * @brief handle hub disconnect
 */
void Application::handleHubDisconnect() {
  this->trayMenu->setJoinedToHub(false);
}

/**
 * @brief On Hub Error Occurred
 */
void Application::handleHubErrorOccurred(QAbstractSocket::SocketError error) {
  this->trayIcon->showMessage(
    constants::getAppName(),
    QObject::tr("Hub Connection Error Occurred: %1").arg(QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey(error)),
    QIcon(QString::fromStdString(constants::getAppLogo()))
  );
}

//----------------------------- slots for Tray ----------------------------//

/**
 * @brief On Qr Code Clicked
 */
void Application::openQrCode() {
  // if already visible return
  if (group->isVisible()) { return group->raise(); }

  // generate the qr code with all inteface ip and port in format
  auto interfaces = QNetworkInterface::allInterfaces();

  // get server info
  auto server = controller->getServerInfo();

  // address
  const auto addrs = QNetworkInterface::allAddresses();

  // construct json object { "port": 1234, "ips": [...]  }
  QJsonObject json;

  // add port
  json.insert("port", server.port);

  // add ips
  QJsonArray ips;

  // using for loop
  for (auto addr: addrs) {
    // if not ipv4 then skip
    if (addr.protocol() == QAbstractSocket::IPv6Protocol) {
      continue;
    }

    // if not localhost
    if (addr.isLoopback()) {
      continue;
    }

    // if localhost skip
    if (addr.toString().startsWith("127.")) {
      continue;
    }

    // add the ip to array
    ips.append(addr.toString());
  }

  // add the ips to json
  json.insert("ips", ips);

  // json to string
  auto info = QJsonDocument(json).toJson(QJsonDocument::Compact);

  // log
  qDebug() << "QR Code Info: " << QString(info);

  // set the icon
  group->setWindowIcon(QIcon(constants::getAppLogo()));

  // set the title
  group->setWindowTitle(constants::getAppName());

  // set Fixed Size
  group->setFixedSize(group->sizeHint());

  // set the info
  group->setQrCode(QString(info));

  // set port
  group->setPort(QString::number(server.port));

  // show the dialog
  group->show();

  // set as not resizable
  group->setFixedSize(group->sizeHint());

  // center the window
  group->move(QGuiApplication::primaryScreen()->availableGeometry().center() - group->rect().center());
}

/**
 * @brief On Connect Clicked
 */
void Application::openConnect() {
  // if already visible return
  if (joiner->isVisible()) { return joiner->raise(); }

  // set the icon
  joiner->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  joiner->setWindowTitle(constants::getAppName());

  // set as not resizable
  joiner->setFixedSize(joiner->sizeHint());

  // show the dialog
  joiner->show();

  // center the window
  joiner->move(QGuiApplication::primaryScreen()->availableGeometry().center() - joiner->rect().center());
}

/**
 * @brief On About Clicked
 */
void Application::openAbout() {
  // if already visible return
  if (aboutUs->isVisible()) { return aboutUs->raise(); }

  // set the icon
  aboutUs->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  aboutUs->setWindowTitle(constants::getAppName());

  // set as not resizable
  aboutUs->setFixedSize(aboutUs->sizeHint());

  // show the dialog
  aboutUs->show();

  // center the window
  aboutUs->move(QGuiApplication::primaryScreen()->availableGeometry().center() - aboutUs->rect().center());
}

/**
 * @brief On Open App Clicked
 */
void Application::openClipbird() {
  // set the content Ratio
  this->clipbird->setFixedSize(constants::getAppWindowSize());

  // set the icon to content
  this->clipbird->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  this->clipbird->setWindowTitle(constants::getAppName());

  // show the content
  this->clipbird->show();
}

/**
 * @brief On Send Clicked
 */
void Application::sendClipboard() {
  Q_UNUSED(QtConcurrent::run([this]() {
    auto content = controller->getClipboard();
    QTimer::singleShot(0, controller, [=, this]() {
      this->controller->syncClipboard(content);
    });
  }));
}

/**
 * @brief On Received Clicked
 */
void Application::openHistory() {
  // if already visible return
  if (history->isVisible()) { return history->raise(); }

  // set the icon
  history->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  history->setWindowTitle(constants::getAppName());

  // set history
  history->setHistory(controller->getHistory());

  // set size
  history->setFixedSize(constants::getAppWindowSize());

  // show the dialog
  history->show();
}

/**
 * @brief On Reset Clicked
 */
void Application::resetDevices() {
  controller->clearServerCertificates();
  controller->clearClientCertificates();
}

/**
 * @brief On Account Clicked
 */
void Application::onAccountClicked() {
  if (this->trayMenu->isSignedIn()) {
    utility::functions::toFuture(storage::SecureStorage::instance().removeHubHostDevice())
    .then([=, this]() {
      return utility::functions::toFuture(storage::SecureStorage::instance().removeHubJwtToken());
    })
    .unwrap()
    .then([=, this]() {
      syncing::wan::AuthTokenHolder::instance().setAuthToken(std::nullopt);
    })
    .onFailed([=, this](const std::exception& e) {
      this->trayIcon->showMessage(constants::getAppName(), QObject::tr("Error Signing Out: %1").arg(e.what()), QIcon(QString::fromStdString(constants::getAppLogo())));
    });
    return;
  }

  // if already visible return
  if (signin->isVisible()) { return signin->raise(); }

  // set the icon
  signin->setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  signin->setWindowTitle(constants::getAppName());

  // set as not resizable
  signin->setFixedSize(signin->sizeHint());

  // show the dialog
  signin->show();
}

/**
 * @brief On Hub Clicked
 */
void Application::onHubClicked() {
  if (this->trayMenu->isJoinedToHub()) {
    storage::Storage::instance().setIsUserConnectedToHubLastly(false);
    controller->disconnectFromHub();
    this->trayMenu->setJoinedToHub(false);
    return;
  } else {
    storage::Storage::instance().setIsUserConnectedToHubLastly(true);
    connectToHub();
  }
}

/**
 * @brief On Tray Icon Clicked
 */
void Application::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
    clipbird->isVisible() ? clipbird->hide() : this->openClipbird();
  }
}

/**
 * @brief Set the Qss File for the color scheme
 */
void Application::setQssFile(Qt::ColorScheme scheme) {
  // detect system theme is dark or light
  bool isDark     = scheme == Qt::ColorScheme::Dark;

  // qss
  std::string qss = isDark ? constants::getAppQSSDark() : constants::getAppQSSLight();

  // log
  qInfo()  << META_DATA << "System Theme: " << (isDark ? "Dark" : "Light");
  qDebug() << META_DATA << "QSS File: " << QString::fromStdString(qss);

  // QFile to read the qss file
  QFile qssFile(QString::fromStdString(qss));

  // open the qss file
  qssFile.open(QFile::ReadOnly);

  // set the style sheet
  qApp->setStyleSheet(qssFile.readAll());
}

/**
 * @brief Construct a new Application object
 */
Application::Application(int &argc, char **argv) : SingleApplication(argc, argv) {
  // create the objects of the class
  controller = new controller::ClipBird(this->getSslConfiguration());
  hotkey     = new QHotkey(QKeySequence(constants::getAppHistoryShortcut()), true, this);
  clipbird   = new ui::gui::widgets::Clipbird();
  history    = new ui::gui::widgets::History();
  trayMenu   = new ui::gui::TrayMenu();
  trayIcon   = new QSystemTrayIcon();

  // On HostName successfully resolved
  const auto slot_hr = [=, this](QWidget* dialog, quint16 port, const auto& host) {
    // if host name is not resolved
    if (host.error() != QHostInfo::NoError) {
      return;
    }

    // close the dialog
    dialog->setVisible(false);

    // connect to server
    controller->connectToServer({
      host.addresses().first(), port, host.hostName()
    });
  };

  // validate the ip and port
  const auto validator = [](auto ip, auto port) -> bool {
    if (!QHostAddress(ip).isNull() && port > 0 && port < 65535) {
      return true;
    } else {
      return false;
    }
  };

  utility::functions::toFuture(
    storage::SecureStorage::instance().getHubAuthToken(),
    &storage::ReadAuthTokenDtoJob::authToken
  ).then([=, this](std::optional<syncing::wan::AuthTokenDto> token) {
    if (!token.has_value()) {
      return syncing::wan::AuthTokenHolder::instance().setAuthToken(std::nullopt);
    }
    syncing::wan::AuthTokenHolder::instance().setAuthToken(token.value());
    if (storage::Storage::instance().getHubIsConnectedLastly()) {
      connectToHub();
    }
  }).onFailed([=, this](const std::exception& e) {
    throw std::runtime_error("Error reading Hub JWT Token: " + std::string(e.what()));
  });

  trayIcon->setIcon(QIcon(constants::getAppLogo()));
  trayIcon->setContextMenu(trayMenu);
  trayIcon->setToolTip(constants::getAppName());
  trayIcon->setToolTip(QString::fromStdString(constants::getAppName()));

  // set the signal handler for all os
  signal(SIGTERM, [](int sig) { qApp->quit(); });
  signal(SIGINT, [](int sig) { qApp->quit(); });
  signal(SIGABRT, [](int sig) { qApp->quit(); });

  // set initial theme
  setQssFile(QGuiApplication::styleHints()->colorScheme());

  // set not to quit on last content closed
  qApp->setQuitOnLastWindowClosed(false);

  // tray icon click from content
  QObject::connect(
    trayIcon, &QSystemTrayIcon::activated,
    this, &Application::onTrayIconClicked
  );

  // set the signal for instance Started
  QObject::connect(
    this, &SingleApplication::instanceStarted,
    clipbird, &ui::gui::widgets::Clipbird::show
  );

  // detect the system theme
  QObject::connect(
    QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
    this, &Application::setQssFile
  );

  // set the signal for menus QrCode click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnQrCodeClicked,
    this, &Application::openQrCode
  );

  // set the signal for menus Connect click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnConnectClicked,
    this, &Application::openConnect
  );

  // set the signal for menus About click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnAboutClicked,
    this, &Application::openAbout
  );

  // set the signal for menus Open App click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnOpenAppClicked,
    this, &Application::openClipbird
  );

  // send the signal for menus Received click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnHistoryClicked,
    this, &Application::openHistory
  );

  // set the signal for menus Reset click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnResetClicked,
    this, &Application::resetDevices
  );

  // set the signal for menus Quit click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnExitClicked,
    [this] { QApplication::quit(); }
  );

  // connect the signal for signin
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnAccountClicked,
    this, &Application::onAccountClicked
  );

  // connect the signal for hub
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnHubClicked,
    this, &Application::onHubClicked
  );

  // Connect the signal and slot for tab change(client)
  connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    this, &Application::handleTabChange
  );

  // set the signal for menus Send click
  QObject::connect(
    history, &ui::gui::widgets::History::onClipSend,
    this, &Application::sendClipboard
  );

  // close on tab change
  QObject::connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    group, &QDialog::close
  );

  // close on tab change
  QObject::connect(
    clipbird, &ui::gui::widgets::Clipbird::onTabChanged,
    joiner, &QDialog::close
  );

  // connect signal for history delete
  connect(
    history, &ui::gui::widgets::History::onClipDelete,
    controller, &controller::ClipBird::deleteHistoryAt
  );

  // connect signal for history change
  connect(
    controller, &controller::ClipBird::OnHistoryChanged,
    history, &ui::gui::widgets::History::setHistory
  );

  // connect signal for Clipboard Copy
  connect(
    history, &ui::gui::widgets::History::onClipSelected,
    [=, this](auto i) {
      controller->setClipboard(controller->getHistory().at(i));
    }
  );

  // Connect the signal and slot for client list change
  connect(
    controller, &controller::ClipBird::OnClientListChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleClientListChange
  );

  // Connect the signal and slot for server list change
  connect(
    controller, &controller::ClipBird::OnServerListChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleServerListChange
  );

  // Connect the signal and slot for server status change
  connect(
    controller, &controller::ClipBird::OnMdnsRegisterStatusChangeChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleMdnsRegisterStatusChanged
  );

  // connect signal and slot for OnAuthRequest
  connect(
    controller, &controller::ClipBird::OnAuthRequest,
    this, &Application::handleAuthRequest
  );

  // Connect the signal and slot for server status change
  connect(
    controller, &controller::ClipBird::OnServerStatusChanged,
    clipbird, &ui::gui::widgets::Clipbird::handleServerStatusChanged
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::disconnectFromServer,
    controller, &controller::ClipBird::disconnectFromServer
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::disconnectClient,
    controller, &controller::ClipBird::disconnectClient
  );

  connect(
    clipbird, &ui::gui::widgets::Clipbird::connectToServer,
    controller, &controller::ClipBird::connectToServer
  );

  connect(
    hotkey, &QHotkey::activated,
    this, &Application::openHistory
  );

  connect(
    &syncing::wan::AuthTokenHolder::instance(),
    &syncing::wan::AuthTokenHolder::authTokenChanged,
    [=, this](auto token) {
      this->trayMenu->setHubEnabled(token.has_value());
      this->trayMenu->setSignedIn(token.has_value());
    }
  );

  connect(
    controller, &controller::ClipBird::OnHubConnected,
    this, &Application::handleHubConnect
  );

  connect(
    controller, &controller::ClipBird::OnHubDisconnected,
    this, &Application::handleHubDisconnect
  );

  connect(
    controller, &controller::ClipBird::OnHubErrorOccurred,
    this, &Application::handleHubErrorOccurred
  );

  // connect the dialog to window clicked signal
  connect(joiner, &ui::gui::modals::Connect::onConnect, [=, this](auto ipv4, auto port) {
    // validate the ip and port
    if (!validator(ipv4.toShort(), port.toShort())) {
      return;
    }

    // bind the port
    auto slot = std::bind(
      slot_hr, joiner, port.toShort(), std::placeholders::_1
    );

    // resolve the host name
    QHostInfo::lookupHost(ipv4, this, slot);
  });

  connect(
    signin, &ui::gui::modals::SignIn::onSignIn,
    this, &Application::handleSignin
  );

  // if host is lastly server
  if (controller->isLastlyHostIsServer()) {
    clipbird->setTabAsServer();
  } else {
    clipbird->setTabAsClient();
  }

  // show the tray icon
  trayIcon->show();
}

/**
 * @brief Destroy the Application object
 */
Application::~Application() {
  delete controller;
  delete clipbird;
  delete history;
  delete trayMenu;
  delete trayIcon;
  delete aboutUs;
  delete group;
  delete joiner;
}

/**
 * @brief Get the controller object
 */
controller::ClipBird* Application::getController() const {
  return controller;
}
}
