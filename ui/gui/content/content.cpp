// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "content.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
//------------------------------ slots for Content -------------------------//

/**
 * @brief handle the host action from the window
 */
void Content::handleHostAction(Tabs t, components::Device::Value h) {
  if (t == Tabs::Server && std::get<1>(h) == Action::Disconnect) {
    controller->disconnectClient(std::get<0>(h));
  }

  if (t == Tabs::Client && std::get<1>(h) == Action::Connect) {
    controller->connectToServer(std::get<0>(h));
  }

  if (t == Tabs::Client && std::get<1>(h) == Action::Disconnect) {
    controller->disconnectFromServer(std::get<0>(h));
  }
}

/**
 * @brief On Tab Changed for Client
 */
void Content::handleTabChangeForClient(Tabs tab) {
  if (tab != Tabs::Client) return;  // if not client tab return

  // initialize the client Content
  this->setGroupName(c_groupNameKey, "-");
  this->setStatus(c_statusKey, Status::Disconnected);
  this->setHostCount(c_hostsKey, 0);

  // reset the device list
  this->removeAllClient();

  // update tray menu state
  this->trayMenu->setQrCodeEnabled(false);
  this->trayMenu->setConnectEnabled(true);

  // notify the controller
  controller->setCurrentHostAsClient();
}

/**
 * @brief On Tab Changed for Server
 */
void Content::handleTabChangeForServer(Tabs tab) {
  if (tab != Tabs::Server) return;  // if not server tab return

  // initialize the server Content
  this->setGroupName(s_groupNameKey, "-");
  this->setStatus(s_statusKey, Status::Inactive);
  this->setHostCount(s_hostsKey, 0);

  // reset the device list
  this->removeAllServers();

  // update tray menu state
  this->trayMenu->setConnectEnabled(false);
  this->trayMenu->setQrCodeEnabled(true);

  // notify the controller
  controller->setCurrentHostAsServer();
}

//----------------------------- slots for Server --------------------------//

/**
 * @brief Handle the Client List Item Clicked
 */
void Content::handleClientListChange(QList<types::device::Device> clients) {
  // Create a list of tuple with Action
  QList<components::Device::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);

  // set the host count
  this->setHostCount(s_hostsKey, clients_m.size());
}

/**
 * @brief Handle the Server State Change
 */
void Content::handleServerStateChange(bool isStarted) {
  // infer the status from the server state
  auto groupName  = isStarted ? controller->getServerInfo().name : QString("-");
  auto status_m   = isStarted ? Status::Active : Status::Inactive;
  auto clients    = controller->getConnectedClientsList();

  // set the server status
  this->setGroupName(s_groupNameKey, groupName);
  this->setStatus(s_statusKey, status_m);
  this->setHostCount(s_hostsKey, clients.size());

  // Create a list of tuple with Action
  QList<components::Device::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);
}

/**
 * @brief On New Host Connected
 *
 * @param client
 */
void Content::handleAuthRequest(const types::device::Device& client) {
  // get the message to show
  // clang-format off
  auto message = QString(
    "A New client Attempting to connect\n"
    "Host: %1\n"
    "Accept the connection?"
  ).arg(
    client.name
  );
  // clang-format on

  // get the user input
  auto dialog = new QMessageBox();

  // icon for the dialog
  auto icon = QIcon(constants::getAppLogo().c_str());

  // set the icon
  dialog->setWindowIcon(icon);

  // set the title
  dialog->setWindowTitle("Clipbird");

  // set the message
  dialog->setText(message);

  // set delete on close
  dialog->setAttribute(Qt::WA_DeleteOnClose);

  // set the buttons
  dialog->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

  // set the default button
  dialog->setDefaultButton(QMessageBox::No);

  // show the dialog
  dialog->show();

  // connect the dialog to window AuthSuccess signal
  const auto signal_s = &QMessageBox::accepted;
  const auto slot_s   = [=] { controller->authSuccess(client); };
  connect(dialog, signal_s, slot_s);

  // connect the dialog to window AuthFail signal
  const auto signal_f = &QMessageBox::rejected;
  const auto slot_f   = [=] { controller->authFailed(client); };
  connect(dialog, signal_f, slot_f);
}

//----------------------------- slots for Client --------------------------//

/**
 * @brief Handle the Server List Item Clicked
 */
void Content::handleServerListChange(QList<types::device::Device> servers) {
  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    // if client is connected to server
    if (!controller->isConnectedToServer()) return Action::Connect;

    // get the server
    auto server = controller->getConnectedServer();

    // infer
    if (s.ip == server.ip && s.port == server.port) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  };

  // add the server to the list
  for (auto s : servers) {
    servers_m.append({s, getAction(s)});
  }

  // set the server list to the window
  this->setServerList(servers_m);

  // set the host count
  this->setHostCount(c_hostsKey, servers_m.size());
}

/**
 * @brief Handle the server status change
 */
void Content::handleServerStatusChanged(bool isConnected) {
  // infer the status from the server state
  auto groupName = isConnected ? controller->getConnectedServer().name : QString("-");
  auto servers   = controller->getServerList();
  auto status_m  = isConnected ? Status::Connected : Status::Disconnected;

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    if (!controller->isConnectedToServer()) return Action::Connect;

    // get the server
    auto server = controller->getConnectedServer();

    // infer
    if (s.ip == server.ip && s.port == server.port) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  };

  // set the server status
  this->setGroupName(c_groupNameKey, groupName);
  this->setStatus(c_statusKey, status_m);
  this->setHostCount(c_hostsKey, servers.size());

  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // add the server to the list
  for (auto s : servers) {
    servers_m.append({s, getAction(s)});
  }

  // set the server list to the window
  this->setServerList(servers_m);
}

/**
 * @brief On Qr Code Clicked
 */
void Content::onQrCodeClicked() {
  // generate the qr code with all inteface ip and port in format
  // [ip1;ip2;ip3;...;ipn;]port
  auto interfaces = QNetworkInterface::allInterfaces();

  // get server info
  auto server = controller->getServerInfo();

  // accumulator
  const auto acc = [](const auto& a, const auto& b) {
    return a + b.toString().toStdString() + ";";
  };

  // address
  const auto addrs = QNetworkInterface::allAddresses();

  // accumulate the ip address
  auto info = std::accumulate(addrs.begin(), addrs.end(), std::string("["), acc);

  // add port to the info
  info += "]" + std::to_string(server.port);

  // log
  qDebug() << "QR Code Info: " << info.c_str();

  // create a dialog
  auto dialog = new QDialog();

  // create layout
  auto root = new QVBoxLayout();

  // create label
  auto qrcode = new components::QrCode(dialog);

  // create label
  auto port = new QLabel(dialog);

  // set the size
  qrcode->setFixedSize(200, 200);

  // set the text
  qrcode->setText(info.c_str());

  // set the text
  port->setText(QString::number(server.port));

  // set center alignment
  port->setAlignment(Qt::AlignCenter);

  // set the style sheet
  port->setStyleSheet("QLabel { color : gray; }");

  // add the ip and port input to layout
  root->addWidget(qrcode);
  root->addWidget(port);

  // size policy
  root->setSizeConstraint(QLayout::SetFixedSize);

  // center alignment of qrcode
  root->setAlignment(qrcode, Qt::AlignCenter);

  // set the icon
  dialog->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  dialog->setWindowTitle("Clipbird");

  // set the root layout to dialog
  dialog->setLayout(root);

  // set delete on close
  dialog->setAttribute(Qt::WA_DeleteOnClose);

  // show the dialog
  dialog->show();

  // set as not resizable
  dialog->setFixedSize(dialog->sizeHint());

  // theme changer
  auto onStyleChanged = [=]() {
    if (QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
      qrcode->setColor(Qt::white);
    } else {
      qrcode->setColor(Qt::black);
    }
  };

  // initial theme
  onStyleChanged();

  // detect the system theme
  const auto signal = &QStyleHints::colorSchemeChanged;
  const auto slot = onStyleChanged;
  QObject::connect(QGuiApplication::styleHints(), signal, slot);

  // close on tab change
  const auto signal_tc = &QTabWidget::currentChanged;
  const auto slot_tc   = [=](int) { dialog->close(); };
  QObject::connect(tab, signal_tc, slot_tc);
}

/**
 * @brief On Connect Clicked
 */
void Content::onConnectClicked() {
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

  // get the ip and port from user
  auto dialog = new QDialog();

  // create root layout
  auto root = new QVBoxLayout(dialog);

  // create label
  auto label = new QLabel("Enter the IP and Port of the server", dialog);

  // create the ip and port input
  auto ipv4 = new QLineEdit(dialog);

  // create the ip and port input
  auto port = new QLineEdit(dialog);

  // create the button
  auto button = new QPushButton("Connect");

  // set the placeholder
  ipv4->setPlaceholderText("IPv4");

  // set the placeholder
  port->setPlaceholderText("Port");

  // add the ip and port input to layout
  root->addWidget(label);
  root->addWidget(ipv4);
  root->addWidget(port);
  root->addWidget(button);

  // set the icon
  dialog->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  dialog->setWindowTitle("Clipbird");

  // set the root layout to dialog
  dialog->setLayout(root);

  // set delete on close
  dialog->setAttribute(Qt::WA_DeleteOnClose);

  // show the dialog
  dialog->show();

  // set as not resizable
  dialog->setFixedSize(dialog->size());

  // close on tab change
  const auto signal_tc = &QTabWidget::currentChanged;
  const auto slot_tc   = [=](int) { dialog->close(); };
  QObject::connect(tab, signal_tc, slot_tc);

  // connect the dialog to window clicked signal
  connect(button, &QPushButton::clicked, [=] {
    // get the ip and port
    auto port_s = port->text().toShort();
    auto ipv4_s = ipv4->text();

    // validate the ip and port
    if (!validator(ipv4_s, port_s)) {
      return;
    }

    // bind the port
    auto slot = std::bind(
      slot_hr, dialog, port_s, std::placeholders::_1
    );

    // resolve the host name
    QHostInfo::lookupHost(ipv4_s, this, slot);
  });
}

/**
 * @brief On About Clicked
 */
void Content::onAboutClicked() {
  QDesktopServices::openUrl(QUrl(constants::getAppHomePage().c_str()));
}

/**
 * @brief On Issue Clicked
 */
void Content::onIssueClicked() {
  QDesktopServices::openUrl(QUrl(constants::getAppIssuePage().c_str()));
}

/**
 * @brief On Reset Clicked
 */
void Content::onResetClicked() {
  controller->clearServerCertificates();
  controller->clearClientCertificates();
}

/**
 * @brief Construct a new Content object
 * with parent as QWidget
 *
 * @param c controller
 * @param p parent
 */
Content::Content(Content::ClipBird* c, QWidget* p) : QFrame(p), controller(c) {
  // set no taskbar icon & no window Frame & always on top
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // create the  layout
  QVBoxLayout* root = new QVBoxLayout();

  // add top layout to  layout
  root->addWidget(this->deviceInfo);

  // Set the Expanding & Document Mode
  tab->tabBar()->setExpanding(true);
  tab->tabBar()->setDocumentMode(true);

  // Create QScrollArea
  auto serverArea = new QScrollArea();
  auto clientArea = new QScrollArea();

  // set the widget as Resizable
  serverArea->setWidgetResizable(true);
  clientArea->setWidgetResizable(true);

  // set policy for scroll area
  serverArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  clientArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  // set the widget to scroll area
  serverArea->setWidget(this->serverList);
  clientArea->setWidget(this->clientList);

  // add server list to tab
  tab->addTab(clientArea, s_tabTitle);

  // add client list to tab
  tab->addTab(serverArea, c_tabTitle);

  // add tab to  layout
  root->addWidget(tab);

  // add  layout to this
  this->setLayout(root);

  // connect tab changed signal
  connect(tab, &QTabWidget::currentChanged, [&](int index) {
    emit onTabChanged((currentTab = static_cast<Tabs>(index)));
  });

  // clang-format off
  // server list slot
  auto serverListSlot  = [=](const auto& host) {
    emit onHostAction(Tabs::Client, host);
  };

  // client list slot
  auto clientListSlot  = [=](const auto& host) {
    emit onHostAction(Tabs::Server, host);
  };
  // clang-format on

  // set the icon to tray
  trayIcon->setIcon(QIcon(constants::getAppLogo().c_str()));

  // set the tray menu
  trayIcon->setContextMenu(trayMenu);

  // set tooltip
  trayIcon->setToolTip(constants::getAppName().c_str());

  // set the signal for menus QrCode click
  const auto signal_qc = &ui::gui::content::TrayMenu::OnQrCodeClicked;
  const auto slot_qc   = &Content::onQrCodeClicked;
  QObject::connect(trayMenu, signal_qc, this, slot_qc);

  // set the signal for menus Connect click
  const auto signal_cc = &ui::gui::content::TrayMenu::OnConnectClicked;
  const auto slot_cc   = &Content::onConnectClicked;
  QObject::connect(trayMenu, signal_cc, this, slot_cc);

  // set the signal for menus About click
  const auto signal_ac = &ui::gui::content::TrayMenu::OnAboutClicked;
  const auto slot_ac   = &Content::onAboutClicked;
  QObject::connect(trayMenu, signal_ac, this, slot_ac);

  // set the signal for menus Issue click
  const auto signal_ic = &ui::gui::content::TrayMenu::OnIssueClicked;
  const auto slot_ic   = &Content::onIssueClicked;
  QObject::connect(trayMenu, signal_ic, this, slot_ic);

  // set the signal for menus Reset click
  const auto signal_rc = &ui::gui::content::TrayMenu::OnResetClicked;
  const auto slot_rc   = &Content::onResetClicked;
  QObject::connect(trayMenu, signal_rc, this, slot_rc);

  // set the signal for menus Quit click
  const auto signal_ec = &ui::gui::content::TrayMenu::OnExitClicked;
  const auto slot_ec   = [] { QApplication::quit(); };
  QObject::connect(trayMenu, signal_ec, this, slot_ec);

  // connect server list signal
  const auto signal_so = &content::DeviceList::onAction;
  const auto slot_so   = serverListSlot;
  connect(serverList, signal_so, slot_so);

  // connect client list signal
  const auto signal_co = &content::DeviceList::onAction;
  const auto slot_co   = clientListSlot;
  connect(clientList, signal_co, slot_co);

  // Connect the signal and slot for client list change
  const auto signal_cl = &ClipBird::OnClientListChanged;
  const auto slot_cl   = &Content::handleClientListChange;
  connect(controller, signal_cl, this, slot_cl);

  // Connect the signal and slot for server list change
  const auto signal_sl = &ClipBird::OnServerListChanged;
  const auto slot_sl   = &Content::handleServerListChange;
  connect(controller, signal_sl, this, slot_sl);

  // Connect the signal and slot for server status change
  const auto signal_st = &ClipBird::OnServerStateChanged;
  const auto slot_st   = &Content::handleServerStateChange;
  connect(controller, signal_st, this, slot_st);

  // connect signal and slot for OnAuthRequest
  const auto signal_or = &ClipBird::OnAuthRequest;
  const auto slot_or = &Content::handleAuthRequest;
  connect(controller, signal_or, this, slot_or);

  // Connect the signal and slot for server status change
  const auto signal_ss = &ClipBird::OnServerStatusChanged;
  const auto slot_ss   = &Content::handleServerStatusChanged;
  connect(controller, signal_ss, this, slot_ss);

  // Connect the signal and slot for host action
  const auto signal_ha = &Content::onHostAction;
  const auto slot_ha   = &Content::handleHostAction;
  connect(this, signal_ha, this, slot_ha);

  // Connect the signal and slot for tab change(client)
  const auto signal_tc = &Content::onTabChanged;
  const auto slot_tc   = &Content::handleTabChangeForClient;
  connect(this, signal_tc, this, slot_tc);

  // Connect the signal and slot for tab change(server)
  const auto signal_ts = &Content::onTabChanged;
  const auto slot_ts   = &Content::handleTabChangeForServer;
  connect(this, signal_ts, this, slot_ts);

  // if host is lastly server
  if (controller->isLastlyHostIsServer()) {
    this->onTabChanged(Tabs::Server);
  } else {
    this->setTabAsClient();
  }

  // show tray icon
  trayIcon->show();
}

/**
 * @brief Set the Status object
 */
void Content::setStatus(const QString& key, components::Status::Value val) {
  this->deviceInfo->setHostStatus({key, val});
}

/**
 * @brief Get the Host Status object
 */
QPair<QString, components::Status::Value> Content::getStatus() {
  return this->deviceInfo->getHostStatus();
}

/**
 * @brief Set the Server Name object
 */
void Content::setGroupName(const QString& key, const QString& val) {
  this->deviceInfo->setGroupName({key, val});
}

/**
 * @brief Get the Server Name object
 */
QPair<QString, QString> Content::getServerHostName() {
  return this->deviceInfo->getGroupName();
}

/**
 * @brief Set the Hosts object
 */
void Content::setHostCount(const QString& key, int val) {
  this->deviceInfo->setHostCount({key, QString::number(val)});
}

/**
 * @brief Get the Hosts object
 */
QPair<QString, int> Content::getHostCount() {
  const auto hostCount = this->deviceInfo->getHostCount();
  return {hostCount.first, hostCount.second.toInt()};
}

/**
 * @brief Set tab as client
 */
void Content::setTabAsClient() {
  this->tab->setCurrentIndex(1);
}

/**
 * @brief Set tab as server
 */
void Content::setTabAsServer() {
  this->tab->setCurrentIndex(0);
}

/**
 * @brief Get System Tray Icon
 */
QSystemTrayIcon* Content::getTrayIcon() {
  return this->trayIcon;
}

//---------------------- Server Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Content::setClientList(const QList<components::Device::Value>& hosts) {
  clientList->setHosts(hosts);
}

/**
 * @brief Get the Server List object
 */
QList<components::Device::Value> Content::getClientList() {
  return clientList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Content::addClient(components::Device::Value host) {
  clientList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Content::removeClient(components::Device::Value host) {
  clientList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Content::removeAllClient() {
  clientList->removeHosts();
}

//---------------------- Client Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Content::setServerList(const QList<components::Device::Value>& hosts) {
  serverList->setHosts(hosts);
}

/**
 * @brief Get the Server List from the tab
 */
QList<components::Device::Value> Content::getServerList() {
  return serverList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Content::addServer(components::Device::Value host) {
  serverList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Content::removeServer(components::Device::Value host) {
  serverList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Content::removeAllServers() {
  serverList->removeHosts();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
