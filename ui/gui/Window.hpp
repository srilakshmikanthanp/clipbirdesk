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
#include "ui/gui/components/core/index.hpp"
#include "ui/gui/components/index.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Window : public QWidget {
 private:  // Member variable (Tabs)
  components::HostsList* clientList = new components::HostsList(this); // Server Tab
  components::HostsList* serverList = new components::HostsList(this); // Client Tab

 private:  // Member variable
  components::PairView* hostStatus = new components::PairView(this);
  components::PairView* serverName = new components::PairView(this);
  components::PairView* serverIP = new components::PairView(this);
  components::PairView* hostCount = new components::PairView(this);

 public:   // typedefs used in this class
  using Action = components::HostView::Action;
  using Status = components::StatView::Value;

 private:  // Constants for style
  static constexpr const char* const style = R"(
  )";

 public:   // enum for this class
  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable
  Tabs currentTab = Tabs::Server;

 private:  // just for Qt
  Q_OBJECT

 signals:  // signals
  void onHostAction(Tabs tab, std::tuple<QHostAddress, quint16, Action>);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 public:
  /**
   * @brief Construct a new Window object
   * with parent as QWidget
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
    auto tab = new components::core::Tabbed(this);

    // add server list to tab
    tab->addTab(serverList, "Server");

    // add client list to tab
    tab->addTab(clientList, "Client");

    // add tab to root layout
    root->addWidget(tab);

    // using some components
    using components::core::Tabbed;
    using components::HostsList;

    // connect tab changed signal
    connect(tab, &Tabbed::currentChanged, [&](int index){
      emit onTabChanged((currentTab = static_cast<Tabs>(index)));
    });

    // server list slot
    auto serverListSlot = [&](auto action) {
      emit onHostAction(Tabs::Server, action);
    };

    // client list slot
    auto clientListSlot = [&](auto action) {
      emit onHostAction(Tabs::Client, action);
    };

    // connect server list signal
    connect(serverList, &HostsList::onAction, serverListSlot);

    // connect client list signal
    connect(clientList, &HostsList::onAction, clientListSlot);

    // set the style sheet
    setStyleSheet(style);
  }

  /**
   * @brief Set the Status object
   */
  void setHostStatus(const QString& key, components::StatView::Value val) {
    // create the components of the class
    auto label = new components::core::Label(this);
    auto status = new components::StatView(this);

    // set the values
    label->setText(key); status->setStatus(val);

    // set the status
    hostStatus->set(label, status);
  }

  /**
   * @brief Get the Host Status object
   */
  QPair<QString, components::StatView::Value> getHostStatus() {
    using components::core::Label;
    using components::StatView;

    const auto status = qobject_cast<StatView*>(hostStatus->get().second);
    const auto label = qobject_cast<Label*>(hostStatus->get().first);

    return {label->text(), status->getStatus()};
  }

  /**
   * @brief Set the Server Name object
   */
  void setServerName(const QString& key, const QString& val) {
    auto label = new components::core::Label(this);
    auto info = new components::core::Label(this);

    label->setText(key); info->setText(val);

    serverName->set(label, info);
  }

  /**
   * @brief Get the Server Name object
   */
  QPair<QString, QString> getServerName() {
    using components::core::Label;

    const auto label = qobject_cast<Label*>(serverName->get().first);
    const auto info = qobject_cast<Label*>(serverName->get().second);

    return {label->text(), info->text()};
  }

  /**
   * @brief Set the Server Ip object
   */
  void setServerIp(const QString& key, const QString& val) {
    auto label = new components::core::Label(this);
    auto info = new components::core::Label(this);

    label->setText(key); info->setText(val);

    serverIP->set(label, info);
  }

  /**
   * @brief Get the Server Ip object
   */
  QPair<QString, QString> getServerIp() {
    using components::core::Label;

    const auto label = qobject_cast<Label*>(serverIP->get().first);
    const auto info = qobject_cast<Label*>(serverIP->get().second);

    return {label->text(), info->text()};
  }

  /**
   * @brief Set the Hosts object
   */
  void setHostCount(const QString& key, int val) {
    auto label = new components::core::Label(this);
    auto info = new components::core::Label(this);

    label->setText(key); info->setText(QString::number(val));

    hostCount->set(label, info);
  }

  /**
   * @brief Get the Hosts object
   */
  QPair<QString, int> getHostCount() {
    using components::core::Label;

    const auto label = qobject_cast<Label*>(hostCount->get().first);
    const auto info = qobject_cast<Label*>(hostCount->get().second);

    return {label->text(), info->text().toInt()};
  }

  //---------------------- Server Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setClientList(QList<std::tuple<QHostAddress, quint16, Action>> hosts) {
    serverList->setHosts(hosts);
  }

  /**
   * @brief Get the Server List object
   */
  QList<std::tuple<QHostAddress, quint16, Action>> getClientList() {
    return serverList->getAllHosts();
  }

  /**
   * @brief Add Server to the list
   */
  void addClient(std::tuple<QHostAddress, quint16, Action> host) {
    serverList->addHost(host);
  }

  /**
   * @brief Remove a Server from the list
   */
  void removeClient(std::tuple<QHostAddress, quint16, Action> host) {
    serverList->removeHost(host);
  }

  /**
   * @brief Remove all servers from the list
   */
  void removeAllClient() {
    serverList->removeAllHosts();
  }

  //---------------------- Client Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setServerList(QList<std::tuple<QHostAddress, quint16, Action>> hosts) {
    clientList->setHosts(hosts);
  }

  /**
   * @brief Get the Server List from the tab
   */
  QList<std::tuple<QHostAddress, quint16, Action>> getServerList() {
    return clientList->getAllHosts();
  }

  /**
   * @brief Add Server to the list
   */
  void addServer(std::tuple<QHostAddress, quint16, Action> host) {
    clientList->addHost(host);
  }

  /**
   * @brief Remove a Server from the list
   */
  void removeServer(std::tuple<QHostAddress, quint16, Action> host) {
    clientList->removeHost(host);
  }

  /**
   * @brief Remove all servers from the list
   */
  void removeAllServers() {
    clientList->removeAllHosts();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
