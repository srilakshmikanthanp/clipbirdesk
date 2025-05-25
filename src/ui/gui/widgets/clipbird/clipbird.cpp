#include "clipbird.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets {
/**
 * @brief handle the host action from the window
 */
void Clipbird::handleHostAction(Tabs t, components::HostTile::Value h) {
  if (t == Tabs::Server && std::get<1>(h) == Action::Disconnect) {
    emit disconnectClient(std::get<0>(h));
  }

  if (t == Tabs::Client && std::get<1>(h) == Action::Connect) {
    emit connectToServer(std::get<0>(h));
  }

  if (t == Tabs::Client && std::get<1>(h) == Action::Disconnect) {
    emit disconnectFromServer(std::get<0>(h));
  }
}

/**
 * @brief Handle the Client List Item Clicked
 */
void Clipbird::handleClientListChange(QList<types::Device> clients) {
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
  if (tab != ui::gui::widgets::Clipbird::Tabs::Client) return;  // if not client tab return
  this->removeAllClient();
}

/**
 * @brief On Tab Changed for Server
 */
void Clipbird::handleTabChangeForServer(Tabs tab) {
  if (tab != ui::gui::widgets::Clipbird::Tabs::Server) return;  // if not server tab return
  auto name = QString::fromStdString(constants::getMDnsServiceName());
  this->removeAllServers();
}

/**
 * @brief Handle the Server State Change
 */
void Clipbird::handleMdnsRegisterStatusChange(bool isRegistered) {

}

/**
 * @brief Handle the Server List Item Clicked
 */
void Clipbird::handleServerListChange(std::optional<types::Device> server, QList<types::Device> servers) {
  // Create a list of tuple with Action
  QList<components::HostTile::Value> serversTile;

  // get the action for the server
  const auto getAction = [=](const auto& s) {
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
void Clipbird::handleServerStatusChanged(bool isConnected, types::Device server) {
  // get the servers
  auto servers = this->getServerList();

  // get action for the server
  const auto getAction = [=](const auto& s) {
    if (std::get<0>(s).ip == server.ip && std::get<0>(s).port == server.port) {
      return isConnected ? Action::Disconnect : Action::Connect;
    } else {
      return std::get<1>(s);
    }
  };

  // update the server list
  this->setServerList(
    std::accumulate(
      servers.begin(), servers.end(), QList<components::HostTile::Value>(),
      [&](auto acc, const auto& s) {
        acc.append({std::get<0>(s), getAction(s)});
        return acc;
      }
    )
  );
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
Clipbird::Clipbird(QWidget* p) : QWidget(p) {
  // create the  layout
  QVBoxLayout* root = new QVBoxLayout();

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
    this->handleHostAction(Tabs::Client, host);
  };

  // client list slot
  auto clientListSlot  = [=](const auto& host) {
    this->handleHostAction(Tabs::Server, host);
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
}

/**
 * @brief Set tab as client
 */
void Clipbird::setTabAsClient() {
  // if already just emit the signal
  if (currentTab == Tabs::Client) {
    emit onTabChanged(Tabs::Client);
    return;
  }

  this->tab->setCurrentIndex(1);
}

/**
 * @brief Set tab as server
 */
void Clipbird::setTabAsServer() {
  // if already just emit the signal
  if (currentTab == Tabs::Server) {
    emit onTabChanged(Tabs::Server);
    return;
  }

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
