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
void Window::handleHostAction(Tabs t, components::Host::Value h) {
  if (t == Tabs::Server && std::get<2>(h) == Action::Disconnect) {
    controller->disconnectFromServer({std::get<0>(h), std::get<1>(h)});
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
  this->setServerName(c_nameKey, "-");
  this->setServerIp(c_ipKey, "-");
  this->setHostCount(c_hostsKey, 0);

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
  this->setServerName(s_nameKey, "-");
  this->setServerIp(s_ipKey, "-");
  this->setHostCount(s_hostsKey, 0);

  // notify the controller
  controller->setCurrentHostAsServer();
}

//----------------------------- slots for Server --------------------------//

/**
 * @brief Handle the Client List Item Clicked
 */
void Window::handleClientListChange(QList<QPair<QHostAddress, quint16>> clients) {
  // Create a list of tuple with Action
  QList<components::Host::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c.first, c.second, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);

  // set the host count
  this->setHostCount(s_hostsKey, clients_m.size());
}

/**
 * @brief Handle the Server State Change
 */
void Window::handleServerStateChange(bool isStarted) {
  // infer the status from the server state
  const auto status = isStarted ? Status::Active : Status::Inactive;

  // set the server status
  this->setStatus(s_statusKey, status);
}

//----------------------------- slots for Client --------------------------//

/**
 * @brief Handle the Server List Item Clicked
 */
void Window::handleServerListChange(QList<QPair<QHostAddress, quint16>> servers) {
  // Create a list of tuple with Action
  QList<components::Host::Value> servers_m;

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
  this->setHostCount(c_hostsKey, servers_m.size());
}

/**
 * @brief Handle the server status change
 */
void Window::handleServerStatusChange(bool status) {
  // infer the status from the server state
  const auto status_m = status ? Status::Connected : Status::Disconnected;

  // set the server status
  this->setStatus(c_statusKey, status_m);
}

/**
 * @brief Construct a new Window object
 * with parent as QWidget
 */
Window::Window(QWidget* parent) : QWidget(parent) {
  // set no taskbar icon & no window Frame
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

  // create all the components
  this->hostStatus.first  = new components::Label(this);
  this->hostStatus.second = new components::Status(this);

  this->serverName.first  = new components::Label(this);
  this->serverName.second = new components::Label(this);

  this->serverIp.first    = new components::Label(this);
  this->serverIp.second   = new components::Label(this);

  this->hostCount.first   = new components::Label(this);
  this->hostCount.second  = new components::Label(this);

  // set Alignment
  this->hostStatus.first->setAlignment(Qt::AlignLeft);
  this->hostStatus.second->setAlignment(Qt::AlignRight);

  this->serverName.first->setAlignment(Qt::AlignLeft);
  this->serverName.second->setAlignment(Qt::AlignRight);

  this->serverIp.first->setAlignment(Qt::AlignLeft);
  this->serverIp.second->setAlignment(Qt::AlignRight);

  this->hostCount.first->setAlignment(Qt::AlignLeft);
  this->hostCount.second->setAlignment(Qt::AlignRight);

  // create the root layout
  auto root = new QVBoxLayout(this);

  // create the top layout
  auto top  = new QFormLayout();

  // add top layout components
  top->addRow(this->hostStatus.first, this->hostStatus.second);
  top->addRow(this->serverName.first, this->serverName.second);
  top->addRow(this->serverIp.first, this->serverIp.second);
  top->addRow(this->hostCount.first, this->hostCount.second);

  // set top layout alignment
  top->setLabelAlignment(Qt::AlignLeft);
  top->setFormAlignment(Qt::AlignRight);

  // add top layout to root layout
  root->addLayout(top);

  // create tab widget
  auto tab = new components::Tab(this);

  // add server list to tab
  tab->addTab(serverList, "Server");

  // add client list to tab
  tab->addTab(clientList, "Client");

  // add tab to root layout
  root->addWidget(tab);

  // using some components
  using components::HostsList;
  using components::Tab;

  // connect tab changed signal
  connect(tab, &Tab::currentChanged, [&](int index) {
    emit onTabChanged((currentTab = static_cast<Tabs>(index)));
  });

  // server list slot
  auto serverListSlot = [&](auto host) { emit onHostAction(Tabs::Server, host); };

  // client list slot
  auto clientListSlot = [&](auto host) { emit onHostAction(Tabs::Client, host); };

  // connect server list signal
  connect(serverList, &HostsList::onAction, serverListSlot);

  // connect client list signal
  connect(clientList, &HostsList::onAction, clientListSlot);

  // Connect the signal and slot for client list change
  const auto signal_cl = &Controller::OnClientListChanged;
  const auto slot_cl   = &Window::handleClientListChange;
  connect(controller, signal_cl, this, slot_cl);

  // Connect the signal and slot for server list change
  const auto signal_sl = &Controller::OnServerListChanged;
  const auto slot_sl   = &Window::handleServerListChange;
  connect(controller, signal_sl, this, slot_sl);

  // Connect the signal and slot for server status change
  const auto signal_ss = &Controller::OnServerStateChanged;
  const auto slot_ss   = &Window::handleServerStatusChange;
  connect(controller, signal_ss, this, slot_ss);

  // Connect the signal and slot for server status change
  const auto signal_sc = &Controller::OnServerStateChanged;
  const auto slot_sc   = &Window::handleServerStateChange;
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

  // Initialize the Tab as Client
  tab->setCurrentIndex(1);

  // set the style sheet
  setStyleSheet(style);
}

/**
 * @brief Set the Status object
 */
void Window::setStatus(const QString& key, components::Status::Value val) {
  this->hostStatus.first->setText(key);
  this->hostStatus.second->setStatus(val);
}

/**
 * @brief Get the Host Status object
 */
QPair<QString, components::Status::Value> Window::getStatus() {
  return {this->hostStatus.first->text(), this->hostStatus.second->getStatus()};
}

/**
 * @brief Set the Server Name object
 */
void Window::setServerName(const QString& key, const QString& val) {
  this->serverName.first->setText(key);
  this->serverName.second->setText(val);
}

/**
 * @brief Get the Server Name object
 */
QPair<QString, QString> Window::getServerName() {
  return {this->serverName.first->text(), this->serverName.second->text()};
}

/**
 * @brief Set the Server Ip object
 */
void Window::setServerIp(const QString& key, const QString& val) {
  this->serverIp.first->setText(key);
  this->serverIp.second->setText(val);
}

/**
 * @brief Get the Server Ip object
 */
QPair<QString, QString> Window::getServerIp() {
  return {this->serverIp.first->text(), this->serverIp.second->text()};
}

/**
 * @brief Set the Hosts object
 */
void Window::setHostCount(const QString& key, int val) {
  this->hostCount.first->setText(key);
  this->hostCount.second->setText(QString::number(val));
}

/**
 * @brief Get the Hosts object
 */
QPair<QString, int> Window::getHostCount() {
  return {this->hostCount.first->text(), this->hostCount.second->text().toInt()};
}

//---------------------- Server Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Window::setClientList(QList<components::Host::Value> hosts) {
  serverList->setHosts(hosts);
}

/**
 * @brief Get the Server List object
 */
QList<components::Host::Value> Window::getClientList() {
  return serverList->getAllHosts();
}

/**
 * @brief Add Server to the list
 */
void Window::addClient(components::Host::Value host) {
  serverList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Window::removeClient(components::Host::Value host) {
  serverList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Window::removeAllClient() {
  serverList->removeAllHosts();
}

//---------------------- Client Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Window::setServerList(QList<components::Host::Value> hosts) {
  clientList->setHosts(hosts);
}

/**
 * @brief Get the Server List from the tab
 */
QList<components::Host::Value> Window::getServerList() {
  return clientList->getAllHosts();
}

/**
 * @brief Add Server to the list
 */
void Window::addServer(components::Host::Value host) {
  clientList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Window::removeServer(components::Host::Value host) {
  clientList->removeHost(host);
}

/**
 * @brief Remove all servers from the list
 */
void Window::removeAllServers() {
  clientList->removeAllHosts();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
