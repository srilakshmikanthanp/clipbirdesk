// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
//------------------------------ slots for Clipbird -------------------------//

/**
 * @brief handle the host action from the window
 */
void Clipbird::handleHostAction(Tabs t, components::Host::Value h) {
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
void Clipbird::handleTabChangeForClient(Tabs tab) {
  if (tab != Tabs::Client) return;  // if not client tab return

  // initialize the client Clipbird
  this->setStatus("Join to a Group", Status::Disconnected);

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
void Clipbird::handleTabChangeForServer(Tabs tab) {
  if (tab != Tabs::Server) return;  // if not server tab return

  // Device mDns name
  auto name = QString::fromStdString(constants::getMDnsServiceName());

  // initialize the server Clipbird
  this->setStatus(name, Status::Inactive);

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
void Clipbird::handleClientListChange(QList<types::device::Device> clients) {
  // Create a list of tuple with Action
  QList<components::Host::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);
}

/**
 * @brief Handle the Server State Change
 */
void Clipbird::handleServerStateChange(bool isStarted) {
  // infer the status from the server state
  auto groupName  = QString::fromStdString(constants::getMDnsServiceName());
  auto status_m   = isStarted ? Status::Active : Status::Inactive;
  auto clients    = controller->getConnectedClientsList();

  // If the server is started
  if (isStarted) {
    groupName = controller->getServerInfo().name;
  }

  // set the server status
  this->setStatus(groupName, status_m);

  // Create a list of tuple with Action
  QList<components::Host::Value> clients_m;

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
void Clipbird::handleAuthRequest(const types::device::Device& client) {
  // get the message to show
  // clang-format off
  auto message = QObject::tr(
    "A New Host Wants to Join Group\n"
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
  dialog->setWindowTitle(constants::getAppName().c_str());

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

  // set fixed size
  dialog->setFixedSize(dialog->sizeHint());

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
void Clipbird::handleServerListChange(QList<types::device::Device> servers) {
  // Create a list of tuple with Action
  QList<components::Host::Value> servers_m;

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    if (!controller->isConnectedToServer()) return Action::Connect;
    auto server = controller->getConnectedServer();
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
}

/**
 * @brief Handle the server status change
 */
void Clipbird::handleServerStatusChanged(bool isConnected) {
  // infer the status from the server state
  auto groupName = isConnected ? controller->getConnectedServer().name : QString("Join to a Group");
  auto servers   = controller->getServerList();
  auto status_m  = isConnected ? Status::Connected : Status::Disconnected;

  // server compare
  const auto serverCompare = [=](const auto& device) -> bool {
    if (!controller->isConnectedToServer()) false;
    auto server = controller->getConnectedServer();
    return server.ip == std::get<0>(device).ip && server.port == std::get<0>(device).port;
  };

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    if (!controller->isConnectedToServer()) return Action::Connect;
    auto server = controller->getConnectedServer();
    if (s.ip == server.ip && s.port == server.port) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  };

  // Create a list of tuple with Action
  QList<components::Host::Value> servers_m;

  // set the server status
  this->setStatus(groupName, status_m);

  // add the server to the list
  for (auto s : servers) {
    servers_m.append({s, getAction(s)});
  }

  // if the server is not in the list
  if (isConnected && !std::any_of(servers_m.begin(), servers_m.end(), serverCompare)) {
    servers_m.append({controller->getConnectedServer(), Action::Disconnect});
  }

  // set the server list to the window
  this->setServerList(servers_m);
}

/**
 * @brief handle onConnectionError
 */
void Clipbird::handleConnectionError(QString error) {
  // Just Show the error info to user via Dialog
  auto message = QString("Connection Error: %1").arg(error);

  // Title of the Notification
  auto title = constants::getAppName().c_str();

  // icon for the dialog
  auto icon = QIcon(constants::getAppLogo().c_str());

  // show notification
  trayIcon->showMessage(title, message, icon);
}

//----------------------------- slots for Tray ----------------------------//

/**
 * @brief On Qr Code Clicked
 */
void Clipbird::onQrCodeClicked() {
  // if already visible return
  if (group.isVisible()) return;

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
  group.setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  group.setWindowTitle(constants::getAppName().c_str());

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

  // close on tab change
  QObject::connect(tab, &QTabWidget::currentChanged, &group, &QDialog::close);
}

/**
 * @brief On Connect Clicked
 */
void Clipbird::onConnectClicked() {
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

  // if already visible return
  if (joiner.isVisible()) return;

  // set the icon
  joiner.setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  joiner.setWindowTitle(constants::getAppName().c_str());

  // set as not resizable
  joiner.setFixedSize(joiner.sizeHint());

  // close on tab change
  QObject::connect(tab, &QTabWidget::currentChanged, &joiner, &QDialog::close);

  // show the dialog
  if (!joiner.isVisible()) joiner.show();

  // connect the dialog to window clicked signal
  connect(&joiner, &modals::Connect::onConnect, [=](auto ipv4, auto port) {
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
}

/**
 * @brief On About Clicked
 */
void Clipbird::onAboutClicked() {
  // if already visible return
  if (aboutUs.isVisible()) return;

  // set the icon
  aboutUs.setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  aboutUs.setWindowTitle(constants::getAppName().c_str());

  // show the dialog
  aboutUs.show();

  // close on tab change
  QObject::connect(tab, &QTabWidget::currentChanged, &aboutUs, &QDialog::close);
}

/**
 * @brief On Reset Clicked
 */
void Clipbird::onResetClicked() {
  controller->clearServerCertificates();
  controller->clearClientCertificates();
}

/**
 * @brief On Open App Clicked
 */
void Clipbird::onOpenAppClicked() {
  this->window()->show();
}

/**
 * @brief On Send Clicked
 */
void Clipbird::onSendClicked() {
  this->controller->syncClipboard(controller->getClipboard());
}

/**
 * @brief On Received Clicked
 */
void Clipbird::onReceivedClicked() {
  // if already visible return
  if (history.isVisible()) return;

  // set the icon
  history.setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  history.setWindowTitle(constants::getAppName().c_str());

  // set history
  history.setHistory(controller->getHistory());

  // set size
  history.setFixedSize(QSize(350, 380));

  // connect signal for Clipboard Copy
  const auto signal_cc = &modals::History::onClipSelected;
  const auto slot_cc   = [=](auto i) { controller->setClipboard(controller->getHistory().at(i)); };
  connect(&history, signal_cc, slot_cc);

  // connect signal for history change
  const auto signal_hc = &ClipBird::OnHistoryChanged;
  const auto slot_hc   = &modals::History::setHistory;
  connect(controller, signal_hc, &history, slot_hc);

  // connect signal for history delete
  const auto signal_hd = &modals::History::onClipDelete;
  const auto slot_hd   = &ClipBird::deleteHistoryAt;
  connect(&history, signal_hd, controller, slot_hd);

  // show the dialog
  history.show();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Clipbird::setUpLanguage() {
  this->tab->setTabText(0, QObject::tr(this->s_tabTitle));
  this->tab->setTabText(1, QObject::tr(this->c_tabTitle));
}

/**
 * @brief Construct a new Clipbird object
 * with parent as QWidget
 *
 * @param c controller
 * @param p parent
 */
Clipbird::Clipbird(Clipbird::ClipBird* c, QWidget* p) : QFrame(p), controller(c) {
  // set no taskbar icon & no window Frame & always on top
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // create the  layout
  QVBoxLayout* root = new QVBoxLayout();

  // status text alignment center
  this->status->setAlignment(Qt::AlignCenter);

  // add top layout to layout
  root->addWidget(this->status);

  // set status center both horizontally and vertically
  root->setAlignment(this->status, Qt::AlignCenter);

  // set the cursor as arrow
  tab->tabBar()->setCursor(Qt::PointingHandCursor);

  // Set the Expanding & Document Mode
  tab->tabBar()->setDocumentMode(true);
  tab->tabBar()->setExpanding(true);

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

  // Align center the List
  serverArea->setAlignment(Qt::AlignCenter);
  clientArea->setAlignment(Qt::AlignCenter);

  // add server list to tab
  tab->addTab(clientArea, QObject::tr(s_tabTitle));

  // add client list to tab
  tab->addTab(serverArea, QObject::tr(c_tabTitle));

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

  // set the signal for menus QrCode click
  const auto signal_qc = &ui::gui::TrayMenu::OnQrCodeClicked;
  const auto slot_qc   = &Clipbird::onQrCodeClicked;
  QObject::connect(trayMenu, signal_qc, this, slot_qc);

  // set the signal for menus Connect click
  const auto signal_cc = &ui::gui::TrayMenu::OnConnectClicked;
  const auto slot_cc   = &Clipbird::onConnectClicked;
  QObject::connect(trayMenu, signal_cc, this, slot_cc);

  // set the signal for menus About click
  const auto signal_ac = &ui::gui::TrayMenu::OnAboutClicked;
  const auto slot_ac   = &Clipbird::onAboutClicked;
  QObject::connect(trayMenu, signal_ac, this, slot_ac);

  // set the signal for menus Open App click
  const auto signal_oc = &ui::gui::TrayMenu::OnOpenAppClicked;
  const auto slot_oc   = &Clipbird::onOpenAppClicked;
  QObject::connect(trayMenu, signal_oc, this, slot_oc);

  // set the signal for menus Send click
  const auto signal_sc = &ui::gui::TrayMenu::OnSendClicked;
  const auto slot_sc   = &Clipbird::onSendClicked;
  QObject::connect(trayMenu, signal_sc, this, slot_sc);

  // send the signal for menus Received click
  const auto signal_rcv = &ui::gui::TrayMenu::OnHistoryClicked;
  const auto slot_rcv   = &Clipbird::onReceivedClicked;
  QObject::connect(trayMenu, signal_rcv, this, slot_rcv);

  // set the signal for menus Reset click
  const auto signal_rc = &ui::gui::TrayMenu::OnResetClicked;
  const auto slot_rc   = &Clipbird::onResetClicked;
  QObject::connect(trayMenu, signal_rc, this, slot_rc);

  // set the signal for menus Quit click
  const auto signal_ec = &ui::gui::TrayMenu::OnExitClicked;
  const auto slot_ec   = [] { QApplication::quit(); };
  QObject::connect(trayMenu, signal_ec, this, slot_ec);

  // connect server list signal
  const auto signal_so = &components::HostList::onAction;
  const auto slot_so   = serverListSlot;
  connect(serverList, signal_so, slot_so);

  // connect client list signal
  const auto signal_co = &components::HostList::onAction;
  const auto slot_co   = clientListSlot;
  connect(clientList, signal_co, slot_co);

  // Connect the signal and slot for client list change
  const auto signal_cl = &ClipBird::OnClientListChanged;
  const auto slot_cl   = &Clipbird::handleClientListChange;
  connect(controller, signal_cl, this, slot_cl);

  // Connect the signal and slot for server list change
  const auto signal_sl = &ClipBird::OnServerListChanged;
  const auto slot_sl   = &Clipbird::handleServerListChange;
  connect(controller, signal_sl, this, slot_sl);

  // Connect the signal and slot for server status change
  const auto signal_st = &ClipBird::OnServerStateChanged;
  const auto slot_st   = &Clipbird::handleServerStateChange;
  connect(controller, signal_st, this, slot_st);

  // connect signal and slot for OnAuthRequest
  const auto signal_or = &ClipBird::OnAuthRequest;
  const auto slot_or = &Clipbird::handleAuthRequest;
  connect(controller, signal_or, this, slot_or);

  // Connect the signal and slot for server status change
  const auto signal_ss = &ClipBird::OnServerStatusChanged;
  const auto slot_ss   = &Clipbird::handleServerStatusChanged;
  connect(controller, signal_ss, this, slot_ss);

  // Connect the signal and slot for host action
  const auto signal_ha = &Clipbird::onHostAction;
  const auto slot_ha   = &Clipbird::handleHostAction;
  connect(this, signal_ha, this, slot_ha);

  // Connect the signal and slot for tab change(client)
  const auto signal_tc = &Clipbird::onTabChanged;
  const auto slot_tc   = &Clipbird::handleTabChangeForClient;
  connect(this, signal_tc, this, slot_tc);

  // Connect the signal and slot for tab change(server)
  const auto signal_ts = &Clipbird::onTabChanged;
  const auto slot_ts   = &Clipbird::handleTabChangeForServer;
  connect(this, signal_ts, this, slot_ts);

  // Connect the signal and slot for OnConnectionError
  const auto signal_ce = &ClipBird::OnConnectionError;
  const auto slot_ce   = &Clipbird::handleConnectionError;
  connect(controller, signal_ce, this, slot_ce);

  // if host is lastly server
  if (controller->isLastlyHostIsServer()) {
    this->onTabChanged(Tabs::Server);
  } else {
    this->setTabAsClient();
  }
}

/**
 * @brief Set the Status object
 */
void Clipbird::setStatus(const QString& key, components::Status::Value val) {
  this->status->set(key, val);
}

/**
 * @brief Get the Host Status object
 */
QPair<QString, components::Status::Value> Clipbird::getStatus() {
  return this->status->get();
}

/**
 * @brief Set tab as client
 */
void Clipbird::setTabAsClient() {
  this->tab->setCurrentIndex(1);
}

/**
 * @brief Set tab as server
 */
void Clipbird::setTabAsServer() {
  this->tab->setCurrentIndex(0);
}

/**
 * @brief Set the QSystemTrayIcon
 */
void Clipbird::setTrayIcon(QSystemTrayIcon* trayIcon) {
  this->trayIcon = trayIcon;

  // set the icon to tray
  trayIcon->setIcon(QIcon(constants::getAppLogo().c_str()));

  // set the tray menu
  trayIcon->setContextMenu(trayMenu);

  // set tooltip
  trayIcon->setToolTip(constants::getAppName().c_str());

  // show tray icon
  trayIcon->show();
}

/**
 * @brief Get System Tray Icon
 */
QSystemTrayIcon* Clipbird::getTrayIcon() {
  return this->trayIcon;
}

//---------------------- Server Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Clipbird::setClientList(const QList<components::Host::Value>& hosts) {
  clientList->setHosts(hosts);
}

/**
 * @brief Get the Server List object
 */
QList<components::Host::Value> Clipbird::getClientList() {
  return clientList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Clipbird::addClient(components::Host::Value host) {
  clientList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Clipbird::removeClient(components::Host::Value host) {
  clientList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Clipbird::removeAllClient() {
  clientList->removeHosts();
}

//---------------------- Client Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Clipbird::setServerList(const QList<components::Host::Value>& hosts) {
  serverList->setHosts(hosts);
}

/**
 * @brief Get the Server List from the tab
 */
QList<components::Host::Value> Clipbird::getServerList() {
  return serverList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Clipbird::addServer(components::Host::Value host) {
  serverList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Clipbird::removeServer(components::Host::Value host) {
  serverList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Clipbird::removeAllServers() {
  serverList->removeHosts();
}

/**
 * @brief change event
 */
void Clipbird::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
