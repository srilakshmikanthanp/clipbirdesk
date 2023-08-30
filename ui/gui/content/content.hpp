#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QApplication>
#include <QDesktopServices>
#include <QFocusEvent>
#include <QFormLayout>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollArea>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>

// standard headers
#include <algorithm>
#include <tuple>
#include <variant>

// project headers
#include "controller/clipbird/clipbird.hpp"
#include "types/device/device.hpp"
#include "ui/gui/components/device/device.hpp"
#include "ui/gui/components/status/status.hpp"
#include "ui/gui/content/deviceinfo/deviceinfo.hpp"
#include "ui/gui/content/devicelist/devicelist.hpp"
#include "ui/gui/content/traymenu/traymenu.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Content : public QFrame {
 private:  // Member variable (Tray)

  ui::gui::content::TrayMenu *trayMenu = new ui::gui::content::TrayMenu(this);  // Tray Menu
  QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);      // Tray Icon

 private:  // Member variable

  content::DeviceInfo* deviceInfo = new content::DeviceInfo();  // Device Info

 private:  // Member variable (Tabs)

  content::DeviceList* clientList = new content::DeviceList();  // Server Tab
  content::DeviceList* serverList = new content::DeviceList();  // Client Tab

 private:  // Member variable (Layout)

  QTabWidget* tab = new QTabWidget();

 public:  // typedefs used in this class

  using Status = components::Status::Value;
  using Action = components::Device::Action;

 private:  // typedefs used in this class

  using ClipBird = controller::ClipBird;

 private:  // Disable copy and move

  Q_DISABLE_COPY_MOVE(Content)

 public:  // enum for this class

  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable

  Tabs currentTab = Tabs::Server;

 private:  // just for Qt

  Q_OBJECT

 private:  // Member Variables

  ClipBird* controller;

 signals:  // signals
  void onHostAction(Tabs tab, std::tuple<types::device::Device, Action>);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 private:  // constant for keys (Server)

  static constexpr const char* const s_groupNameKey = "Group Name";
  static constexpr const char* const s_statusKey    = "Status";
  static constexpr const char* const s_hostsKey     = "Devices";
  static constexpr const char* const s_tabTitle     = "Create Group";

 private:  // constant for keys (Client)

  static constexpr const char* const c_groupNameKey = "Group Name";
  static constexpr const char* const c_statusKey    = "Status";
  static constexpr const char* const c_hostsKey     = "Groups";
  static constexpr const char* const c_tabTitle     = "Join Group";

 private:  // private slots

  //------------------------------ slots for Content -------------------------//

  /**
   * @brief handle the host action from the window
   */
  void handleHostAction(Tabs t, std::tuple<types::device::Device, Action> h);

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
  void handleClientListChange(QList<types::device::Device> clients);

  /**
   * @brief Handle the Server State Change
   */
  void handleServerStateChange(bool isStarted);

  /**
   * @brief On New Host Connected
   *
   * @param client
   */
  void handleNewHostConnected(const types::device::Device &client);

  //----------------------------- slots for Client --------------------------//

  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<types::device::Device> servers);

  /**
   * @brief Handle the Server Authentication
   */
  void handleServerAuthentication(bool isAuthed);

  /**
   * @brief Handle the server status change
   */
  void handleServerStatusChanged(bool status);

  //----------------------------- slots for Tray ----------------------------//

  /**
   * @brief On About Clicked
   */
  void onAboutClicked();

  /**
   * @brief On Issue Clicked
   */
  void onIssueClicked();

  /**
   * @brief On Reset Clicked
   */
  void onResetClicked();

 public:

  /**
   * @brief Construct a new Content object
   * with parent as QWidget
   */
  explicit Content(ClipBird* controller, QWidget* parent = nullptr);

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
  void setGroupName(const QString& key, const QString& val);

  /**
   * @brief Get the Server Name object
   */
  QPair<QString, QString> getServerHostName();

  /**
   * @brief Set the Hosts object
   */
  void setHostCount(const QString& key, int val);

  /**
   * @brief Get the Hosts object
   */
  QPair<QString, int> getHostCount();

  /**
   * @brief Set tab as client
   */
  void setTabAsClient();

  /**
   * @brief Set tab as server
   */
  void setTabAsServer();

  /**
   * @brief Get System Tray Icon
   */
  QSystemTrayIcon* getTrayIcon();

  //---------------------- Server Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setClientList(const QList<std::tuple<types::device::Device, Action>>& hosts);

  /**
   * @brief Get the Server List object
   */
  QList<std::tuple<types::device::Device, Action>> getClientList();

  /**
   * @brief Add Server to the list
   */
  void addClient(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeClient(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllClient();

  //---------------------- Client Tab ----------------------//

  /**
   * @brief Set the Server List object
   */
  void setServerList(const QList<std::tuple<types::device::Device, Action>>& hosts);

  /**
   * @brief Get the Server List from the tab
   */
  QList<std::tuple<types::device::Device, Action>> getServerList();

  /**
   * @brief Add Server to the list
   */
  void addServer(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove a Server from the list
   */
  void removeServer(std::tuple<types::device::Device, Action> host);

  /**
   * @brief Remove all servers from the list
   */
  void removeAllServers();
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
