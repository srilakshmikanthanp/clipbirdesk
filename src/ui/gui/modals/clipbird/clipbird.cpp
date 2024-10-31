// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
//------------------------------ slots for Clipbird -------------------------//

/**
 * @brief handle the host action from the window
 */
void Clipbird::handleHostAction(Tabs t, components::HostTile::Value h) {
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

//----------------------------- slots for Server --------------------------//

/**
 * @brief Handle the Client List Item Clicked
 */
void Clipbird::handleClientListChange(QList<types::device::Device> clients) {
  // Create a list of tuple with Action
  QList<components::HostTile::Value> clients_m;

  // Add the clients to the list
  for (auto c : clients) {
    clients_m.append({c, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clients_m);
}

/**
 * @brief On Tab Changed for Client
 */
void Clipbird::handleTabChangeForClient(Tabs tab) {
  if (tab != ui::gui::modals::Clipbird::Tabs::Client) return;  // if not client tab return

  // initialize the client Clipbird
  this->setStatus(QObject::tr("Not Connected"), ui::gui::modals::Clipbird::Status::Disconnected);

  // reset the device list
  this->removeAllClient();

  // notify the controller
  controller->setCurrentHostAsClient();
}

/**
 * @brief On Tab Changed for Server
 */
void Clipbird::handleTabChangeForServer(Tabs tab) {
  if (tab != ui::gui::modals::Clipbird::Tabs::Server) return;  // if not server tab return

  // Device mDns name
  auto name = QString::fromStdString(constants::getMDnsServiceName());

  // initialize the server Clipbird
  this->setStatus(name, ui::gui::modals::Clipbird::Status::Inactive);

  // reset the device list
  this->removeAllServers();

  // notify the controller
  controller->setCurrentHostAsServer();
}

/**
 * @brief Handle the Server State Change
 */
void Clipbird::handleServerStateChange(bool isStarted) {
  // infer the status from the server state
  auto groupName  = QString::fromStdString(constants::getMDnsServiceName());
  auto status     = isStarted ? Status::Active : Status::Inactive;
  auto clients    = controller->getConnectedClientsList();

  // If the server is started
  if (isStarted) {
    groupName = controller->getServerInfo().name;
  }

  // set the server status
  this->setStatus(groupName, status);

  // Create a list of tuple with Action
  QList<components::HostTile::Value> clientTiles;

  // Add the clients to the list
  for (auto c : clients) {
    clientTiles.append({c, Action::Disconnect});
  }

  // set the client list to the window
  this->setClientList(clientTiles);
}

/**
 * @brief On New Host Connected
 *
 * @param client
 */
void Clipbird::handleAuthRequest(const types::device::Device& client) {
  // get the user input
  notify::JoinRequest* toast = new notify::JoinRequest(this);

  // connect the dialog to window AuthSuccess signal
  const auto connectionAccept = connect(
    toast, &notify::JoinRequest::onAccept,
    [=] { controller->authSuccess(client); }
  );

  connect(
    toast, &notify::JoinRequest::onAccept,
    toast, &QObject::deleteLater
  );

  // disconnect all signals on tab change signal
  connect(this, &Clipbird::onTabChanged, [=]{
    QObject::disconnect(connectionAccept);
  });

  // connect the dialog to window AuthFail signal
  const auto connectionReject   = connect(
    toast, &notify::JoinRequest::onReject,
    [=] { controller->authFailed(client); }
  );

  connect(
    toast, &notify::JoinRequest::onReject,
    toast, &QObject::deleteLater
  );

  // disconnect all signals on tab change signal
  connect(this, &Clipbird::onTabChanged, [=]{
    QObject::disconnect(connectionReject);
  });

  // shoe the notification
  toast->show(client);
}

//----------------------------- slots for Client --------------------------//

/**
 * @brief Handle the Server List Item Clicked
 */
void Clipbird::handleServerListChange(QList<types::device::Device> servers) {
  // Create a list of tuple with Action
  QList<components::HostTile::Value> serversTile;

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    auto server = controller->getConnectedServer();
    if (!server.has_value()) return Action::Connect;
    if (s.ip == server->ip && s.port == server->port) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  };

  // add the server to the list
  for (auto s : servers) {
    serversTile.append({s, getAction(s)});
  }

  // show the connected server always on top
  std::sort(serversTile.begin(), serversTile.end(), [=](const auto& a, const auto& b) {
    return std::get<1>(a) != Action::Disconnect;
  });

  // set the server list to the window
  this->setServerList(serversTile);
}

/**
 * @brief Handle the server status change
 */
void Clipbird::handleServerStatusChanged(bool isConnected) {
  // get the connected server
  auto server = controller->getConnectedServer();

  // if server is not connected
  isConnected = isConnected && server.has_value();

  // infer the status from the server state
  auto groupName = isConnected ? server->name : QObject::tr("Not Connected");
  auto servers   = controller->getServerList();
  auto status    = isConnected ? Status::Connected : Status::Disconnected;

  // server compare
  const auto serverCompare = [=](const auto& device) -> bool {
    if (isConnected) {
      return server->ip == device.ip && server->port == device.port;
    } else {
      return false;
    }
  };

  // get the action for the server
  const auto getAction = [=](const auto& s) {
    if (serverCompare(s)) {
      return Action::Disconnect;
    } else {
      return Action::Connect;
    }
  };

  // Create a list of tuple with Action
  QList<components::HostTile::Value> serversTiles;

  // set the server status
  this->setStatus(groupName, status);

  // add the server to the list
  for (auto s : servers) {
    serversTiles.append({s, getAction(s)});
  }

  // if the server is not in the list
  if (isConnected && !std::any_of(serversTiles.begin(), serversTiles.end(), [=](const auto& s) {
    return serverCompare(std::get<0>(s));
  })) {
    serversTiles.append({server.value(), Action::Disconnect});
  }

  // show the connected server always on top
  std::sort(serversTiles.begin(), serversTiles.end(), [=](const auto& a, const auto& b) {
    return std::get<1>(a) != Action::Disconnect;
  });

  // set the server list to the window
  this->setServerList(serversTiles);
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Clipbird::setUpLanguage() {
  this->tab->setTabText(0, QObject::tr("Create Group"));
  this->tab->setTabText(1, QObject::tr("Join Group"));
}

/**
 * @brief Construct a new Clipbird object
 * with parent as QWidget
 *
 * @param c controller
 * @param p parent
 */
Clipbird::Clipbird(Clipbird::ClipBird* c, QWidget* p) : QDialog(p), controller(c) {
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
  tab->addTab(clientArea, "");

  // add client list to tab
  tab->addTab(serverArea, "");

  // set up language
  this->setUpLanguage();

  // add tab to  layout
  root->addWidget(tab);

  // add  layout to this
  this->setLayout(root);

  // set object name
  this->setObjectName("Clipbird");

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

  // connect server list signal
  connect(
    serverList, &components::HostList::onAction, serverListSlot
  );

  // connect client list signal
  connect(
    clientList, &components::HostList::onAction, clientListSlot
  );

  // Connect the signal and slot for tab change(client)
  connect(
    this, &Clipbird::onTabChanged,
    this, &Clipbird::handleTabChangeForClient
  );

  // Connect the signal and slot for tab change(server)
  connect(
    this, &Clipbird::onTabChanged,
    this, &Clipbird::handleTabChangeForServer
  );

  // Connect the signal and slot for client list change
  connect(
    controller, &ClipBird::OnClientListChanged,
    this, &Clipbird::handleClientListChange
  );

  // Connect the signal and slot for server list change
  connect(
    controller, &ClipBird::OnServerListChanged,
    this, &Clipbird::handleServerListChange
  );

  // Connect the signal and slot for server status change
  connect(
    controller, &ClipBird::OnServerStateChanged,
    this, &Clipbird::handleServerStateChange
  );

  // connect signal and slot for OnAuthRequest
  connect(
    controller, &ClipBird::OnAuthRequest,
    this, &Clipbird::handleAuthRequest
  );

  // Connect the signal and slot for server status change
  connect(
    controller, &ClipBird::OnServerStatusChanged,
    this, &Clipbird::handleServerStatusChanged
  );

  // Connect the signal and slot for host action
  connect(
    this, &Clipbird::onHostAction,
    this, &Clipbird::handleHostAction
  );

  // init tab as current tab
  if (tab->currentIndex() == 0) {
    emit onTabChanged((currentTab = Tabs::Client));
  } else {
    emit onTabChanged((currentTab = Tabs::Server));
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

//---------------------- Server Tab ----------------------//

/**
 * @brief Set the Server List object
 */
void Clipbird::setClientList(const QList<components::HostTile::Value>& hosts) {
  clientList->setHosts(hosts);
}

/**
 * @brief Get the Server List object
 */
QList<components::HostTile::Value> Clipbird::getClientList() {
  return clientList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Clipbird::addClient(components::HostTile::Value host) {
  clientList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Clipbird::removeClient(components::HostTile::Value host) {
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
void Clipbird::setServerList(const QList<components::HostTile::Value>& hosts) {
  serverList->setHosts(hosts);
}

/**
 * @brief Get the Server List from the tab
 */
QList<components::HostTile::Value> Clipbird::getServerList() {
  return serverList->getHosts();
}

/**
 * @brief Add Server to the list
 */
void Clipbird::addServer(components::HostTile::Value host) {
  serverList->addHost(host);
}

/**
 * @brief Remove a Server from the list
 */
void Clipbird::removeServer(components::HostTile::Value host) {
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

/**
 * @brief override set visible
 */
void Clipbird::setVisible(bool visible) {
  // call base class setVisible
  QWidget::setVisible(visible);

  // if not visible
  if (!visible) return;

  // get the screen size
  auto screen = QGuiApplication::primaryScreen()->availableGeometry();

  // get the widget size
  auto size = this->size();

  // create a new geometry
  QRect geometry;

  // set the geometry to right bottom with margin 20
  geometry.setHeight(size.height());
  geometry.setY(screen.height() - size.height() - 5);
  geometry.setX(screen.width() - size.width() - 5);
  geometry.setWidth(size.width());

  // set the geometry
  this->setGeometry(geometry);
}

/**
 * @brief paint event
 */
void Clipbird::paintEvent(QPaintEvent *event) {
  // For style Sheet take effect
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(
    QStyle::PE_Widget, &opt, &p, this
  );
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
