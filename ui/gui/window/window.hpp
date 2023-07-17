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
#include "controller/controller.hpp"
#include "ui/gui/components/button/button.hpp"
#include "ui/gui/components/label/label.hpp"
#include "ui/gui/components/tab/tab.hpp"
#include "ui/gui/components/hostlist/hostslist.hpp"
#include "ui/gui/components/host/host.hpp"
#include "ui/gui/components/twin/twin.hpp"
#include "ui/gui/components/status/status.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Window : public QWidget {
 private:  // Member variable (Tabs)
  components::HostsList* clientList = new components::HostsList(this); // Server Tab
  components::HostsList* serverList = new components::HostsList(this); // Client Tab

 private:  // Member variable
  components::Twin* hostStatus = new components::Twin(this);
  components::Twin* serverName = new components::Twin(this);
  components::Twin* serverIP = new components::Twin(this);
  components::Twin* hostCount = new components::Twin(this);

 private:  // typedefs used in this class
  using Controller = controller::Controller;

 public:   // typedefs used in this class
  using Status = components::Status::Value;
  using Action = components::Host::Action;

 private:  // Disable copy and move
  Q_DISABLE_COPY_MOVE(Window)

 private:  // Constants for style
  static constexpr const char* const style = R"(
  )";

 public:   // enum for this class
  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable
  Tabs currentTab = Tabs::Server;

 private:  // just for Qt
  Q_OBJECT

 private:  // Member Variables
  Controller *controller;

 signals:  // signals
  void onHostAction(Tabs tab, std::tuple<QHostAddress, quint16, Action>);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 private:  // constant for keys (Server)
  static constexpr const char* const s_statusKey = "Server Status";
  static constexpr const char* const s_ipKey = "Server IP";
  static constexpr const char* const s_nameKey = "Server Name";
  static constexpr const char* const s_hostsKey = "Clients";

 private:  // constant for keys (Client)
  static constexpr const char* const c_statusKey = "Connection Status";
  static constexpr const char* const c_ipKey = "Server IP";
  static constexpr const char* const c_nameKey = "Server Name";
  static constexpr const char* const c_hostsKey = "servers";

 private:  // private slots
  //------------------------------ slots for Window -------------------------//

  /**
   * @brief handle the host action from the window
   */
  void handleHostAction(Tabs t, std::tuple<QHostAddress, quint16, Action> h);

  /**
   * @brief On Tab Changed for Client
   */
  void handleTabChangeForClient(Tabs tab);

  /**
   * @brief On Tab Changed for Server
   */
  void handleTabChangeForServer(Tabs tab);

  //----------------------------- slots for Server --------------------------//

  /**
   * @brief Handle the Client List Item Clicked
   */
  void handleClientListChange(QList<QPair<QHostAddress, quint16>> clients);

  /**
   * @brief Handle the Server State Change
   */
  void handleServerStateChange(bool isStarted);

  //----------------------------- slots for Client --------------------------//

  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<QPair<QHostAddress, quint16>> servers);

  /**
   * @brief Handle the server status change
   */
  void handleServerStatusChange(bool status);

 public:
  /**
   * @brief Construct a new Window object
   * with parent as QWidget
   */
  explicit Window(Controller *controller, QWidget* parent = nullptr);

  /**
   * @brief Set the Status object
   */
  void setStatus(const QString& key, components::Status::Value val);

  /**
   * @brief Get the Host Status object
   */
  QPair<QString, components::Status::Value> getStatus();

  /**
   * @brief Set the Server Name object
   */
  void setServerName(const QString& key, const QString& val);

  /**
   * @brief Get the Server Name object
   */
  QPair<QString, QString> getServerName();

  /**
   * @brief Set the Server Ip object
   */
  void setServerIp(const QString& key, const QString& val);

  /**
   * @brief Get the Server Ip object
   */
  QPair<QString, QString> getServerIp();

  /**
   * @brief Set the Hosts object
   */
  void setHostCount(const QString& key, int val);

  /**
   * @brief Get the Hosts object
   */
  QPair<QString, int> getHostCount();

  //---------------------- Server Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setClientList(QList<std::tuple<QHostAddress, quint16, Action>> hosts);

  /**
   * @brief Get the Server List object
   */
  QList<std::tuple<QHostAddress, quint16, Action>> getClientList();

  /**
   * @brief Add Server to the list
   */
  void addClient(std::tuple<QHostAddress, quint16, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeClient(std::tuple<QHostAddress, quint16, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllClient();

  //---------------------- Client Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setServerList(QList<std::tuple<QHostAddress, quint16, Action>> hosts);

  /**
   * @brief Get the Server List from the tab
   */
  QList<std::tuple<QHostAddress, quint16, Action>> getServerList();

  /**
   * @brief Add Server to the list
   */
  void addServer(std::tuple<QHostAddress, quint16, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeServer(std::tuple<QHostAddress, quint16, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllServers();
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
