// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "window.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
//------------------------------ slots for Window -------------------------//

/**
 * @brief handle the host action from the window
 */
void Window::handleHostAction(Tabs t, components::Device::Value h) {
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
void Window::handleTabChangeForClient(Tabs tab) {
  if (tab != Tabs::Client) return;  // if not client tab return

  // initialize the client Window
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
void Window::handleTabChangeForServer(Tabs tab) {
  if (tab != Tabs::Server) return;  // if not server tab return

  // initialize the server Window
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
void Window::handleClientListChange(QList<QPair<QHostAddress, quint16>> clients) {
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
void Window::handleServerStateChange(bool isStarted) {
  const auto status_m   = isStarted ? Status::Active : Status::Inactive;
  const auto serverInfo = controller->getServerInfo();
  const auto serverIp   = serverInfo.first.toString();
  const auto serverPort = serverInfo.second;
  const auto IpPort     = QString("%1:%2").arg(serverIp).arg(serverPort);
  const auto serverName = QHostInfo::fromName(serverIp).hostName();
  const auto clients    = controller->getConnectedClientsList();

  this->setServerHostName(s_hostNameKey, serverName);
  this->setStatus(s_statusKey, status_m);
  this->setServerIpPort(s_ipPortKey, IpPort);
  this->setHostCount(s_clientsKey, clients.size());
}

/**
 * @brief On New Host Connected
 *
 * @param client
 */
void Window::handleNewHostConnected(const QPair<QHostAddress, quint16>& client) {
  // get the message to show
  auto message = QString("A client is trying to connect to the server\n"
                         "Host: %1:%2\n"
                         "Accept the connection?")
                     .arg(client.first.toString(), QString::number(client.second));

  // get the user input
  auto dialog = new QMessageBox();

  // set the icon
  dialog->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

  // set the title
  dialog->setWindowTitle("Clipbird");

  // set the message
  dialog->setText(message);

  // set the buttons
  dialog->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

  // set the default button
  dialog->setDefaultButton(QMessageBox::No);

  // set delete on close
  dialog->setAttribute(Qt::WA_DeleteOnClose);

  // show the dialog
  dialog->show();

  // connect the dialog to window AuthSuccess signal
  const auto signal_s = &QMessageBox::accepted;
  const auto slot_s   = [&] { controller->authSuccess(client); };
  connect(dialog, signal_s, slot_s);

  // connect the dialog to window AuthFail signal
  const auto signal_f = &QMessageBox::rejected;
  const auto slot_f   = [&] { controller->authFailed(client); };
  connect(dialog, signal_f, slot_f);

  controller->authSuccess(client);
}

//----------------------------- slots for Client --------------------------//

/**
 * @brief Handle the Server List Item Clicked
 */
void Window::handleServerListChange(QList<QPair<QHostAddress, quint16>> servers) {
  // Create a list of tuple with Action
  QList<components::Device::Value> servers_m;

  // get the connected server
  const auto server    = controller->getConnectedServer();

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
 * @brief Handle the server status change
 */
void Window::handleServerStatusChange(bool status) {
  // infer the status from the server state
  const auto status_m   = status ? Status::Connected : Status::Disconnected;
  const auto server     = controller->getConnectedServer();
  const auto serverIp   = server.first.toString();
  const auto serverPort = server.second;
  const auto IpPort     = QString("%1:%2").arg(serverIp).arg(serverPort);
  const auto serverName = QHostInfo::fromName(serverIp).hostName();
  const auto servers    = controller->getServerList();

  // set the server status
  this->setServerHostName(c_hostNameKey, serverName);
  this->setStatus(c_statusKey, status_m);
  this->setServerIpPort(c_ipPortKey, IpPort);
  this->setHostCount(c_serversKey, servers.size());
}

/**
 * @brief Construct a new Window object
 * with parent as QWidget
 */
Window::Window(Window::ClipBird* controller, QWidget* parent)
    : QFrame(parent), controller(controller) {
  // set no taskbar icon & no window Frame & always on top
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // create the  layout
  auto root = new QVBoxLayout();

  // add top layout to  layout
  root->addWidget(this->deviceInfo);

  // create tab widget
  auto tab = new QTabWidget();

  // Set the Expanding & Document Mode
  tab->tabBar()->setExpanding(true);
  tab->tabBar()->setDocumentMode(true);

  // Create QScrollArea
  auto serverListArea = new QScrollArea();
  auto clientListArea = new QScrollArea();

  // set the widget as Resizable
  serverListArea->setWidgetResizable(true);
  clientListArea->setWidgetResizable(true);

  // set the widget to scroll area
  serverListArea->setWidget(this->serverList);
  clientListArea->setWidget(this->clientList);

  // add server list to tab
  tab->addTab(clientListArea, "Server");

  // add client list to tab
  tab->addTab(serverListArea, "Client");

  // add tab to  layout
  root->addWidget(tab);

  // add  layout to this
  this->setLayout(root);

  // connect tab changed signal
  connect(tab, &QTabWidget::currentChanged, [&](int index) {
    emit onTabChanged((currentTab = static_cast<Tabs>(index)));
  });

  // server list slot
  auto serverListSlot = [&](const auto& host) { emit onHostAction(Tabs::Client, host); };

  // client list slot
  auto clientListSlot = [&](const auto& host) { emit onHostAction(Tabs::Server, host); };

  // connect server list signal
  connect(serverList, &window::DeviceList::onAction, serverListSlot);

  // connect client list signal
  connect(clientList, &window::DeviceList::onAction, clientListSlot);

  // Connect the signal and slot for client list change
  const auto signal_cl = &ClipBird::OnClientListChanged;
  const auto slot_cl   = &Window::handleClientListChange;
  connect(controller, signal_cl, this, slot_cl);

  // Connect the signal and slot for server list change
  const auto signal_sl = &ClipBird::OnServerListChanged;
  const auto slot_sl   = &Window::handleServerListChange;
  connect(controller, signal_sl, this, slot_sl);

  // Connect the signal and slot for server status change
  const auto signal_ss = &ClipBird::OnServerStateChanged;
  const auto slot_ss   = &Window::handleServerStateChange;
  connect(controller, signal_ss, this, slot_ss);

  // Connect the signal and slot for server status change
  const auto signal_sc = &ClipBird::OnServerStatusChanged;
  const auto slot_sc   = &Window::handleServerStatusChange;
  connect(controller, signal_sc, this, slot_sc);

  // Connect the signal and slot for host action
  const auto signal_ha = &Window::onHostAction;
  const auto slot_ha   = &Window::handleHostAction;
  connect(this, signal_ha, this, slot_ha);

  // Connect the signal and slot for tab change(client)
  const auto signal_tc = &Window::onTabChanged;
  const auto slot_tc   = &Window::handleTabChangeForClient;
  connect(this, signal_tc, this, slot_tc);

  // Connect the signal and slot for tab change(server)
  const auto signal_ts = &Window::onTabChanged;
  const auto slot_ts   = &Window::handleTabChangeForServer;
  connect(this, signal_ts, this, slot_ts);

  // set the signal for on new Host
  const auto signal_h = &controller::ClipBird::OnNewHostConnected;
  const auto slot_h   = &Window::handleNewHostConnected;
  QObject::connect(controller, signal_h, this, slot_h);

  // Initialize the Tab as Client
  tab->setCurrentIndex(1);

  // set the object name
  this->setObjectName("window");
}

/**
 * @brief Set the Status object
 */
void Window::setStatus(const QString& key, components::Status::Value val) {
  this->deviceInfo->setHostStatus({key, val});
}

/**
 * @brief Get the Host Status object
 */
QPair<QString, components::Status::Value> Window::getStatus() {
  return this->deviceInfo->getHostStatus();
}

/**
 * @brief Set the Server Name object
 */
void Window::setServerHostName(const QString& key, const QString& val) {
  this->deviceInfo->setServerName({key, val});
}

/**
 * @brief Get the Server Name object
 */
QPair<QString, QString> Window::getServerHostName() {
  return this->deviceInfo->getServerName();
}

/**
 * @brief Set the Server Ip object
 */
void Window::setServerIpPort(const QString& key, const QString& val) {
  this->deviceInfo->setServerIpPort({key, val});
}

/**
 * @brief Get the Server Ip object
 */
QPair<QString, QString> Window::getServerIpPort() {
  return this->deviceInfo->getServerIpPort();
}

/**
 * @brief Set the Hosts object
 */
void Window::setHostCount(const QString& key, int val) {
  this->deviceInfo->setHostCount({key, QString::number(val)});
}

/**
 * @brief Get the Hosts object
 */
QPair<QString, int> Window::getHostCount() {
  const auto hostCount = this->deviceInfo->getHostCount();
  return {hostCount.first, hostCount.second.toInt()};
}

//---------------------- Server Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Window::setClientList(const QList<components::Device::Value>& hosts) {
  clientList->setHosts(hosts);
}

/**
 * @brief Get the Server List object
 */
QList<components::Device::Value> Window::getClientList() {
  return clientList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Window::addClient(components::Device::Value host) {
  clientList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Window::removeClient(components::Device::Value host) {
  clientList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Window::removeAllClient() {
  clientList->removeHosts();
}

//---------------------- Client Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Window::setServerList(const QList<components::Device::Value>& hosts) {
  serverList->setHosts(hosts);
}

/**
 * @brief Get the Server List from the tab
 */
QList<components::Device::Value> Window::getServerList() {
  return serverList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Window::addServer(components::Device::Value host) {
  serverList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Window::removeServer(components::Device::Value host) {
  serverList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Window::removeAllServers() {
  serverList->removeHosts();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
