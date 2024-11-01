// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "application.hpp"

namespace srilakshmikanthanp::clipbirdesk {

/**
 * @brief Get the certificate from App Home
 */
QSslConfiguration Application::getOldSslConfiguration() {
  auto two_months      = std::chrono::milliseconds(constants::getAppCertExpiryInterval());
  auto& storage        = storage::Storage::instance();

  // read the certificate and key
  QSslCertificate cert = QSslCertificate(storage.getHostCert(), QSsl::Pem);
  QSslKey key          = QSslKey(storage.getHostKey(), QSsl::Rsa);
  QSslConfiguration sslConfig;

  // get cert name
  auto name = cert.subjectInfo(QSslCertificate::CommonName).constFirst();

  // Name is updated
  if (name != QString::fromStdString(constants::getMDnsServiceName())) {
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
  storage.setHostCert(sslConfig.localCertificate().toPem());
  storage.setHostKey(sslConfig.privateKey().toPem());

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

  // log the certificate and key
  qInfo() << "Certificate: ";
  qInfo().noquote() << "\n";
  qInfo() << config.localCertificate().toPem();
  qInfo() << "Key: ";
  qInfo().noquote() << "\n";
  qInfo() << config.privateKey().toPem();

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
 * @brief On Tab Changed for Client
 */
void Application::handleTabChange(ui::gui::modals::Clipbird::Tabs tab) {
  if (tab == ui::gui::modals::Clipbird::Tabs::Client) {
    this->trayMenu->setQrCodeEnabled(false);
    this->trayMenu->setConnectEnabled(true);
  } else {
    this->trayMenu->setConnectEnabled(false);
    this->trayMenu->setQrCodeEnabled(true);
  }
}

//----------------------------- slots for Tray ----------------------------//

/**
 * @brief On Qr Code Clicked
 */
void Application::onQrCodeClicked() {
  // if already visible return
  if (group.isVisible()) { return group.raise(); }

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
  group.setWindowIcon(QIcon(constants::getAppLogo()));

  // set the title
  group.setWindowTitle(constants::getAppName());

  // set Fixed Size
  group.setFixedSize(group.sizeHint());

  // set the info
  group.setQrCode(QString(info));

  // set port
  group.setPort(QString::number(server.port));

  // show the dialog
  group.show();

  // set as not resizable
  group.setFixedSize(group.sizeHint());

  // center the window
  group.move(QGuiApplication::primaryScreen()->availableGeometry().center() - group.rect().center());
}

/**
 * @brief On Connect Clicked
 */
void Application::onConnectClicked() {
  // if already visible return
  if (joiner.isVisible()) { return joiner.raise(); }

  // set the icon
  joiner.setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  joiner.setWindowTitle(constants::getAppName());

  // set as not resizable
  joiner.setFixedSize(joiner.sizeHint());

  // show the dialog
  joiner.show();

  // center the window
  joiner.move(QGuiApplication::primaryScreen()->availableGeometry().center() - joiner.rect().center());
}

/**
 * @brief On About Clicked
 */
void Application::onAboutClicked() {
  // if already visible return
  if (aboutUs.isVisible()) { return aboutUs.raise(); }

  // set the icon
  aboutUs.setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  aboutUs.setWindowTitle(constants::getAppName());

  // show the dialog
  aboutUs.show();

  // center the window
  aboutUs.move(QGuiApplication::primaryScreen()->availableGeometry().center() - aboutUs.rect().center());
}

/**
 * @brief On Open App Clicked
 */
void Application::openClipbird() {
  // set the content Ratio
  this->clipbird->setFixedSize(constants::getAppWindowSize());

  // set the icon to content
  this->clipbird->setWindowIcon(QIcon(constants::getAppLogo()));

  // set the title
  this->clipbird->setWindowTitle(constants::getAppName());

  // show the content
  this->clipbird->show();
}

/**
 * @brief On Send Clicked
 */
void Application::onSendClicked() {
  Q_UNUSED(QtConcurrent::run([this]() {
    auto content = controller->getClipboard();
    QTimer::singleShot(0, controller, [=]() {
      this->controller->syncClipboard(content);
    });
  }));
}

/**
 * @brief On Received Clicked
 */
void Application::onHistoryClicked() {
  // if already visible return
  if (history.isVisible()) { return history.raise(); }

  // set the icon
  history.setWindowIcon(QIcon(QString::fromStdString(constants::getAppLogo())));

  // set the title
  history.setWindowTitle(constants::getAppName());

  // set history
  history.setHistory(controller->getHistory());

  // set size
  history.setFixedSize(constants::getAppWindowSize());

  // show the dialog
  history.show();
}

/**
 * @brief On Reset Clicked
 */
void Application::onResetClicked() {
  controller->clearServerCertificates();
  controller->clearClientCertificates();
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
  clipbird   = new ui::gui::modals::Clipbird(controller);
  trayMenu   = new ui::gui::TrayMenu(clipbird);
  trayIcon   = new QSystemTrayIcon(this);

  // On HostName successfully resolved
  const auto slot_hr = [=](const auto dialog, quint16 port, const auto& host) {
    // if host name is not resolved
    if (host.error() != QHostInfo::NoError) {
      return;
    }

    // close the dialog
    dialog->close();

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

  // set the icon to tray
  trayIcon->setIcon(QIcon(constants::getAppLogo()));

  // set the tray menu
  trayIcon->setContextMenu(trayMenu);

  // set tooltip
  trayIcon->setToolTip(constants::getAppName());

  // set ToolTip
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
    clipbird, &ui::gui::modals::Clipbird::show
  );

  // detect the system theme
  QObject::connect(
    QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
    this, &Application::setQssFile
  );

  // set the signal for menus QrCode click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnQrCodeClicked,
    this, &Application::onQrCodeClicked
  );

  // set the signal for menus Connect click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnConnectClicked,
    this, &Application::onConnectClicked
  );

  // set the signal for menus About click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnAboutClicked,
    this, &Application::onAboutClicked
  );

  // set the signal for menus Open App click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnOpenAppClicked,
    this, &Application::openClipbird
  );

  // set the signal for menus Send click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnSendClicked,
    this, &Application::onSendClicked
  );

  // send the signal for menus Received click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnHistoryClicked,
    this, &Application::onHistoryClicked
  );

  // set the signal for menus Reset click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnResetClicked,
    this, &Application::onResetClicked
  );

  // set the signal for menus Quit click
  QObject::connect(
    trayMenu, &ui::gui::TrayMenu::OnExitClicked,
    [this] { QApplication::quit(); }
  );

  // Connect the signal and slot for tab change(client)
  connect(
    clipbird, &ui::gui::modals::Clipbird::onTabChanged,
    this, &Application::handleTabChange
  );

  // set the signal for menus Send click
  QObject::connect(
    &history, &ui::gui::modals::History::onClipSend,
    this, &Application::onSendClicked
  );

  // close on tab change
  QObject::connect(
    clipbird, &ui::gui::modals::Clipbird::onTabChanged, 
    &group, &QDialog::close
  );

  // close on tab change
  QObject::connect(
    clipbird, &ui::gui::modals::Clipbird::onTabChanged, 
    &joiner, &QDialog::close
  );

  // connect signal for history delete
  connect(
    &history, &ui::gui::modals::History::onClipDelete,
    controller, &controller::ClipBird::deleteHistoryAt
  );

  // connect signal for history change
  connect(
    controller, &controller::ClipBird::OnHistoryChanged,
    &history, &ui::gui::modals::History::setHistory
  );

  // connect signal for Clipboard Copy
  connect(
    &history, &ui::gui::modals::History::onClipSelected,
    [=](auto i) {
      controller->setClipboard(controller->getHistory().at(i));
    }
  );

  // connect the dialog to window clicked signal
  connect(&joiner, &ui::gui::modals::Connect::onConnect, [=](auto ipv4, auto port) {
    // validate the ip and port
    if (!validator(ipv4.toShort(), port.toShort())) {
      return;
    }

    // bind the port
    auto slot = std::bind(
      slot_hr, &joiner, port.toShort(), std::placeholders::_1
    );

    // resolve the host name
    QHostInfo::lookupHost(ipv4, this, slot);
  });

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
  delete trayMenu;
  delete trayIcon;
}

/**
 * @brief Get the controller object
 */
controller::ClipBird* Application::getController() const {
  return controller;
}
}
