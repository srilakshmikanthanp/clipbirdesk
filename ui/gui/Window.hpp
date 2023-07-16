#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

// standard headers
#include <algorithm>
#include <tuple>
#include <variant>

// project headers
#include "ui/gui/components/index.hpp"
#include "ui/gui/components/individual/index.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Window : public QWidget {
 public:   // enum for this class
  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable
  components::Item* hostStatus = new components::Item(this);
  components::Item* serverName = new components::Item(this);
  components::Item* serverIP = new components::Item(this);
  components::Item* hostCount = new components::Item(this);

 private:  // Member variable (Tabs)
  components::HList* clientList = new components::HList(this); // Server Tab
  components::HList* serverList = new components::HList(this); // Client Tab

 private:  // Member variable
  Tabs currentTab = Tabs::Server;

 public:   // Type alias
  using Action = components::Host::Action;

 private:  // just for Qt
  Q_OBJECT

 signals:  // signals
  void onHostAction(Tabs tab, Action action, QString host, QString ip);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 public:
  /**
   * @brief Construct a new Window object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Window(QWidget* parent = nullptr) : QWidget(parent) {
    // create the root layout
    auto root = new QVBoxLayout(this);

    // create the top layout
    auto top = new QVBoxLayout();

    // add top layout components
    top->addWidget(hostStatus);
    top->addWidget(serverName);
    top->addWidget(serverIP);
    top->addWidget(hostCount);

    // add top layout to root layout
    root->addLayout(top);

    // create tab widget
    auto tab = new components::individual::Tabbed(this);

    // add server list to tab
    tab->addTab(serverList, "Server");

    // add client list to tab
    tab->addTab(clientList, "Client");

    // add tab to root layout
    root->addWidget(tab);

    // using some components
    using components::individual::Tabbed;
    using components::HList;

    // server list slot
    auto serverListSlot = [&](Action action, QString host, QString ip) {
      emit onHostAction(Tabs::Server, action, host, ip);
    };

    // client list slot
    auto clientListSlot = [&](Action action, QString host, QString ip) {
      emit onHostAction(Tabs::Client, action, host, ip);
    };

    // connect tab changed signal
    connect(tab, &Tabbed::currentChanged, [&](int index){
      emit onTabChanged((currentTab = static_cast<Tabs>(index)));
    });

    // connect server list signal
    connect(serverList, &HList::onActionClicked, serverListSlot);

    // connect client list signal
    connect(clientList, &HList::onActionClicked, clientListSlot);

    // TODO: set the style sheet
  }

  /**
   * @brief Set the Status object
   *
   * @param key
   * @param val
   */
  void setHostStatus(const QString& key, components::Status::Value val) {
    // create the components of the class
    auto label = new components::individual::Label(this);
    auto status = new components::Status(this);

    // set the values
    label->setText(key); status->setStatus(val);

    // set the status
    hostStatus->set(label, status);
  }

  /**
   * @brief Get the Host Status object
   *
   * @return QPair<QString, QString>
   */
  QPair<QString, components::Status::Value> getHostStatus() {
    using components::individual::Label;
    using components::Status;

    const auto status = qobject_cast<Status*>(hostStatus->get().second);
    const auto label = qobject_cast<Label*>(hostStatus->get().first);

    return {label->text(), status->getStatus()};
  }

  /**
   * @brief Set the Server Name object
   *
   * @param key
   * @param val
   */
  void setServerName(const QString& key, const QString& val) {
    auto label = new components::individual::Label(this);
    auto info = new components::individual::Label(this);

    label->setText(key); info->setText(val);

    serverName->set(label, info);
  }

  /**
   * @brief Get the Server Name object
   *
   * @return QPair<QString, QString>
   */
  QPair<QString, QString> getServerName() {
    using components::individual::Label;

    const auto label = qobject_cast<Label*>(serverName->get().first);
    const auto info = qobject_cast<Label*>(serverName->get().second);

    return {label->text(), info->text()};
  }

  /**
   * @brief Set the Server Ip object
   *
   * @param key
   * @param val
   */
  void setServerIp(const QString& key, const QString& val) {
    auto label = new components::individual::Label(this);
    auto info = new components::individual::Label(this);

    label->setText(key); info->setText(val);

    serverIP->set(label, info);
  }

  /**
   * @brief Get the Server Ip object
   *
   * @return QPair<QString, QString>
   */
  QPair<QString, QString> getServerIp() {
    using components::individual::Label;

    const auto label = qobject_cast<Label*>(serverIP->get().first);
    const auto info = qobject_cast<Label*>(serverIP->get().second);

    return {label->text(), info->text()};
  }

  /**
   * @brief Set the Hosts object
   *
   * @param key
   * @param val
   */
  void setHostCount(const QString& key, int val) {
    auto label = new components::individual::Label(this);
    auto info = new components::individual::Label(this);

    label->setText(key); info->setText(QString::number(val));

    hostCount->set(label, info);
  }

  /**
   * @brief Get the Hosts object
   *
   * @return QPair<QString, QString>
   */
  QPair<QString, int> getHostCount() {
    using components::individual::Label;

    const auto label = qobject_cast<Label*>(hostCount->get().first);
    const auto info = qobject_cast<Label*>(hostCount->get().second);

    return {label->text(), info->text().toInt()};
  }

  //---------------------- Server Tab ----------------------//

  /**
   * @brief Add Server to the list
   *
   * @param hostName
   * @param ip
   */
  void addClient(QString hostName, QString ip) {
    serverList->addHost(hostName, ip, Action::Disconnect);
  }

  /**
   * @brief Remove a Server from the list
   *
   * @param hostName
   * @param ip
   */
  void removeClient(QString hostName, QString ip) {
    serverList->removeHost(hostName, ip);
  }

  /**
   * @brief Get the Server List object
   *
   * @return QList<QPair<QString, QString>>
   */
  QList<QPair<QString, QString>> getClientList() {
    return serverList->getAllHosts();
  }

  /**
   * @brief Remove all servers from the list
   */
  void clearClientList() {
    serverList->removeAllHosts();
  }

  //---------------------- Client Tab ----------------------//

  /**
   * @brief Add Server to the list
   *
   * @param hostName
   * @param ip
   * @param action
   */
  void addServer(QString hostName, QString ip, Action action) {
    clientList->addHost(hostName, ip, action);
  }

  /**
   * @brief Remove a Server from the list
   *
   * @param hostName
   * @param ip
   */
  void removeServer(QString hostName, QString ip) {
    clientList->removeHost(hostName, ip);
  }

  /**
   * @brief Get the Server List from the tab
   *
   * @return QList<QPair<QString, QString>>
   */
  QList<QPair<QString, QString>> getServerList() {
    return clientList->getAllHosts();
  }

  /**
   * @brief Remove all servers from the list
   */
  void clearServerList() {
    clientList->removeAllHosts();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
