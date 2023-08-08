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
  if (t == Tabs::Server && std::get<2>(h) == Action::Disconnect) {
    controller->disconnectClient({std::get<0>(h), std::get<1>(h)});
  }

  if (t == Tabs::Client && std::get<2>(h) == Action::Connect) {
    controller->connectToServer({std::get<0>(h), std::get<1>(h)});
  }

  if (t == Tabs::Client && std::get<2>(h) == Action::Disconnect) {
    controller->disconnectFromServer({std::get<0>(h), std::get<1>(h)});
  }
}

/**
 * @brief On Tab Changed for Client
 */
void Content::handleTabChangeForClient(Tabs tab) {
  if (tab != Tabs::Client) return;  // if not client tab return

  // initialize the client Content
  this->setStatus(c_statusKey, Status::Disconnected);
  this->setServerHostName(c_hostNameKey, "-");
  this->setServerIpPort(c_ipPortKey, "-");
  this->setHostCount(c_serversKey, 0);

  // reset the device list
  this->removeAllClient();

  // notify the controller
  controller->setCurrentHostAsClient();
}

/**
 * @brief On Tab Changed for Server
 */
void Content::handleTabChangeForServer(Tabs tab) {
  if (tab != Tabs::Server) return;  // if not server tab return

  // initialize the server Content
  this->setStatus(s_statusKey, Status::Inactive);
  this->setServerHostName(s_hostNameKey, "-");
  this->setServerIpPort(s_ipPortKey, "-");
  this->setHostCount(s_clientsKey, 0);

  // reset the device list
  this->removeAllServers();

  // notify the controller
  controller->setCurrentHostAsServer();
}

//----------------------------- slots for Server --------------------------//

/**
 * @brief Handle the Client List Item Clicked
 */
void Content::handleClientListChange(QList<QPair<QHostAddress, quint16>> clients) {
  // Create a list of tuple with Action
  QList<components::Device::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c.first, c.second, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);

  // set the host count
  this->setHostCount(s_clientsKey, clients_m.size());
}

/**
 * @brief Handle the Server State Change
 */
void Content::handleServerStateChange(bool isStarted) {
  // infer the status from the server state
  auto status_m   = isStarted ? Status::Active : Status::Inactive;
  auto serverName = QString("-");
  auto ipPort     = QString("-");
  auto clients    = controller->getConnectedClientsList();

  // if the server is started
  if (isStarted) {
    auto info  = controller->getServerInfo();
    auto ip    = info.first.toString();
    auto port  = info.second;
    serverName = QHostInfo::fromName(ip).hostName();
    ipPort     = QString("%1:%2").arg(ip).arg(port);
  }

  // set the server status
  this->setServerHostName(s_hostNameKey, serverName);
  this->setStatus(s_statusKey, status_m);
  this->setServerIpPort(s_ipPortKey, ipPort);
  this->setHostCount(s_clientsKey, clients.size());

  // Create a list of tuple with Action
  QList<components::Device::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c.first, c.second, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);
}

/**
 * @brief On New Host Connected
 *
 * @param client
 */
void Content::handleNewHostConnected(const QPair<QHostAddress, quint16>& client) {
  // callback for host info looked up
  const auto onHostInfoFound = [=](const QHostInfo& info) -> void {
    // get the message to show
    // clang-format off
    auto message = QString(
      "A New client Attempting to connect\n"
      "Host: %1\n"
      "Port: %2\n"
      "Accept the connection?"
    ).arg(
      info.hostName(), QString::number(client.second)
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
  };

  // lookup the host info
  QHostInfo::lookupHost(client.first.toString(), onHostInfoFound);
}

//----------------------------- slots for Client --------------------------//

/**
 * @brief Handle the Server List Item Clicked
 */
void Content::handleServerListChange(QList<QPair<QHostAddress, quint16>> servers) {
  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // get the connected server
  const auto server    = controller->getAuthedServerOrEmpty();

  // get the action for the server
  const auto getAction = [&server](const auto& s) {
    return s == server ? Action::Disconnect : Action::Connect;
  };

  // add the server to the list
  for (auto s : servers) {
    servers_m.append({s.first, s.second, getAction(s)});
  }

  // set the server list to the window
  this->setServerList(servers_m);

  // set the host count
  this->setHostCount(c_serversKey, servers_m.size());
}

/**
 * @brief Handle the server authentication
 */
void Content::handleServerAuthentication(bool isAuthed) {
  // infer the status from the server state
  auto status_m   = isAuthed ? Status::Connected : Status::Disconnected;
  auto ipPort     = QString("-");
  auto serverName = QString("-");
  auto servers    = controller->getServerList();

  // if the client is Authed
  if (isAuthed) {
    auto server = controller->getAuthedServer();
    auto ip     = server.first.toString();
    auto port   = server.second;
    serverName  = QHostInfo::fromName(ip).hostName();
    ipPort      = QString("%1:%2").arg(ip).arg(port);
  }

  // set the server status
  this->setServerHostName(c_hostNameKey, serverName);
  this->setStatus(c_statusKey, status_m);
  this->setServerIpPort(c_ipPortKey, ipPort);
  this->setHostCount(c_serversKey, servers.size());

  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // get the connected server
  const auto server    = controller->getAuthedServerOrEmpty();

  // get the action for the server
  const auto getAction = [&server](const auto& s) {
    return s == server ? Action::Disconnect : Action::Connect;
  };

  // add the server to the list
  for (auto s : servers) {
    servers_m.append({s.first, s.second, getAction(s)});
  }

  // set the server list to the window
  this->setServerList(servers_m);
}

/**
 * @brief Handle the server status change
 */
void Content::handleServerStatusChanged(bool status) {
  if (status) return; // ignore the TSL connection and leave it to authentication

  // if the server is disconnected
  this->setStatus(c_statusKey, Status::Disconnected);
  this->setServerHostName(c_hostNameKey, "-");
  this->setServerIpPort(c_ipPortKey, "-");

  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // get the connected server
  const auto server    = controller->getAuthedServerOrEmpty();

  // get the action for the server
  const auto getAction = [&server](const auto& s) {
    return s == server ? Action::Disconnect : Action::Connect;
  };

  // add the server to the list
  for (auto s : controller->getServerList()) {
    servers_m.append({s.first, s.second, getAction(s)});
  }

  // set the server list to the window
  this->setServerList(servers_m);
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

  // create tab widget
  QTabWidget* tab = new QTabWidget();

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

  // set the signal for on new Host
  const auto signal_nh = &controller::ClipBird::OnNewHostConnected;
  const auto slot_nh   = &Content::handleNewHostConnected;
  QObject::connect(controller, signal_nh, this, slot_nh);

  // connect server list signal
  const auto signal_so = &window::DeviceList::onAction;
  const auto slot_so   = serverListSlot;
  connect(serverList, signal_so, slot_so);

  // connect client list signal
  const auto signal_co = &window::DeviceList::onAction;
  const auto slot_co   = clientListSlot;
  connect(clientList, signal_co, slot_co);

  // Connect the signal and slot for client list change
  const auto signal_cl = &ClipBird::OnClientListChanged;
  const auto slot_cl   = &Content::handleClientListChange;
  connect(controller, signal_cl, this, slot_cl);

  // Connect the signal and slot for server authentication
  const auto signal_sa = &ClipBird::OnServerAuthentication;
  const auto slot_sa   = &Content::handleServerAuthentication;
  connect(controller, signal_sa, this, slot_sa);

  // Connect the signal and slot for server list change
  const auto signal_sl = &ClipBird::OnServerListChanged;
  const auto slot_sl   = &Content::handleServerListChange;
  connect(controller, signal_sl, this, slot_sl);

  // Connect the signal and slot for server status change
  const auto signal_st = &ClipBird::OnServerStateChanged;
  const auto slot_st   = &Content::handleServerStateChange;
  connect(controller, signal_st, this, slot_st);

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

  // Initialize the Tab as Client
  tab->setCurrentIndex(1);
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
void Content::setServerHostName(const QString& key, const QString& val) {
  this->deviceInfo->setServerName({key, val});
}

/**
 * @brief Get the Server Name object
 */
QPair<QString, QString> Content::getServerHostName() {
  return this->deviceInfo->getServerName();
}

/**
 * @brief Set the Server Ip object
 */
void Content::setServerIpPort(const QString& key, const QString& val) {
  this->deviceInfo->setServerIpPort({key, val});
}

/**
 * @brief Get the Server Ip object
 */
QPair<QString, QString> Content::getServerIpPort() {
  return this->deviceInfo->getServerIpPort();
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
