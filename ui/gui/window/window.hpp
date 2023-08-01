#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QApplication>
#include <QFocusEvent>
#include <QFormLayout>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QMessageBox>
#include <QScrollArea>
#include <QScreen>
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
#include "ui/gui/components/device/device.hpp"
#include "ui/gui/components/status/status.hpp"
#include "ui/gui/components/toast/toast.hpp"
#include "ui/gui/window/deviceinfo/deviceinfo.hpp"
#include "ui/gui/window/devicelist/devicelist.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class Window : public QFrame {
 private:  // Member variable

  window::DeviceInfo* deviceInfo = new window::DeviceInfo();  // Device Info

 private:  // Member variable (Tabs)

  window::DeviceList* clientList = new window::DeviceList();  // Server Tab
  window::DeviceList* serverList = new window::DeviceList();  // Client Tab

 public:  // typedefs used in this class

  using Status = components::Status::Value;
  using Action = components::Device::Action;

 private:  // typedefs used in this class

  using ClipBird = controller::ClipBird;

 private:  // Disable copy and move

  Q_DISABLE_COPY_MOVE(Window)

 public:  // enum for this class

  enum class Tabs { Server = 0, Client = 1 };

 private:  // Member variable

  Tabs currentTab = Tabs::Server;

 private:  // just for Qt

  Q_OBJECT

 private:  // Member Variables

  ClipBird* controller;

 signals:  // signals
  void onHostAction(Tabs tab, std::tuple<QHostAddress, quint16, Action>);

 signals:  // signals
  void onTabChanged(Tabs tab);  // emit when tab changed

 private:  // constant for keys (Server)

  static constexpr const char* const s_statusKey   = "Server Status";
  static constexpr const char* const s_hostNameKey = "Server Name";
  static constexpr const char* const s_ipPortKey   = "Server Info";
  static constexpr const char* const s_clientsKey  = "Connected Clients";
  static constexpr const char* const s_tabTitle    = "As Server";

 private:  // constant for keys (Client)

  static constexpr const char* const c_statusKey   = "Connection Status";
  static constexpr const char* const c_hostNameKey = "Server Name";
  static constexpr const char* const c_ipPortKey   = "Server Info";
  static constexpr const char* const c_serversKey  = "Available Servers";
  static constexpr const char* const c_tabTitle    = "As Client";

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

  /**
   * @brief Handle the clipboard sent
   */
  void handleClipboardSent();

  /**
   * @brief Handle the clipboard recv
   */
  void handleClipboardRecv();

  //----------------------------- slots for Server --------------------------//

  /**
   * @brief Handle the Client List Item Clicked
   */
  void handleClientListChange(QList<QPair<QHostAddress, quint16>> clients);

  /**
   * @brief Handle the Server State Change
   */
  void handleServerStateChange(bool isStarted);

  /**
   * @brief On New Host Connected
   *
   * @param client
   */
  void handleNewHostConnected(const QPair<QHostAddress, quint16> &client);

  //----------------------------- slots for Client --------------------------//

  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<QPair<QHostAddress, quint16>> servers);

  /**
   * @brief Handle the server status change
   */
  void handleServerStatusChanged(bool status);

  /**
   * @brief Handle the Server Authentication
   *
   * @param status
   */
  void handleServerAuthentication(bool isAuthed);

 private: // private functions

  /**
   * @brief Reset the Server Info
   */
  void resetServerInfo();

  /**
   * @brief Reset the Client Info
   */
  void resetClientInfo();

 public:

  /**
   * @brief Construct a new Window object
   * with parent as QWidget
   */
  explicit Window(ClipBird* controller, QWidget* parent = nullptr);

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
  void setServerHostName(const QString& key, const QString& val);

  /**
   * @brief Get the Server Name object
   */
  QPair<QString, QString> getServerHostName();

  /**
   * @brief Set the Server Ip object
   */
  void setServerIpPort(const QString& key, const QString& val);

  /**
   * @brief Get the Server Ip object
   */
  QPair<QString, QString> getServerIpPort();

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
  void setClientList(const QList<std::tuple<QHostAddress, quint16, Action>>& hosts);

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
  void setServerList(const QList<std::tuple<QHostAddress, quint16, Action>>& hosts);

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
