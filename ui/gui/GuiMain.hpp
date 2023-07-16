#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QApplication>
#include <QHostAddress>
#include <QHostInfo>
#include <QIcon>
#include <QList>
#include <QPair>
#include <QWidget>

// Project Headers
#include "controller/controller.hpp"
#include "ui/gui/TrayIcon.hpp"
#include "ui/gui/Window.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class GuiMain : public QWidget {
 private:  // typedefs used in this class
  using Controller = controller::Controller;
  using Window = ui::gui::Window;
  using Tabs = Window::Tabs;
  using TrayIcon = ui::gui::TrayIcon;
  using Action = Window::Action;
  using Status = Window::Status;

 private:  // Disable copy and move
  Q_DISABLE_COPY_MOVE(GuiMain)

 private:  // Member Variables
  Controller controller = Controller(QApplication::clipboard(), this);
  Window window = Window(this);
  TrayIcon trayIcon = TrayIcon(this);

 private:  // just for Qt
  Q_OBJECT

 private:  // Constants
  static constexpr const char* const style = R"(
    QWidget {
      background: transparent;
    }
  )";

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
  void handleHostAction(Tabs t, std::tuple<QHostAddress, quint16, Action> h) {
    if (t == Tabs::Server && std::get<2>(h) == Action::Disconnect) {
      controller.disconnectFromServer({std::get<0>(h), std::get<1>(h)});
    } else if (t == Tabs::Client && std::get<2>(h) == Action::Connect) {
      controller.connectToServer({std::get<0>(h), std::get<1>(h)});
    } else if (t == Tabs::Client && std::get<2>(h) == Action::Disconnect) {
      controller.disconnectFromServer({std::get<0>(h), std::get<1>(h)});
    }
  }

  /**
   * @brief On Tab Changed for Client
   */
  void handleTabChangeForClient(Tabs tab) {
    if (tab != Tabs::Client) return;  // if not client tab return

    // initialize the client Window
    window.setHostStatus(c_statusKey, Status::Disconnected);
    window.setServerName(c_nameKey, "-");
    window.setServerIp(c_ipKey, "-");
    window.setHostCount(c_hostsKey, 0);

    // notify the controller
    controller.setCurrentHostAsClient();
  }

  /**
   * @brief On Tab Changed for Server
   */
  void handleTabChangeForServer(Tabs tab) {
    if (tab != Tabs::Server) return;  // if not server tab return

    // initialize the server Window
    window.setHostStatus(s_statusKey, Status::Inactive);
    window.setServerName(s_nameKey, "-");
    window.setServerIp(s_ipKey, "-");
    window.setHostCount(s_hostsKey, 0);

    // notify the controller
    controller.setCurrentHostAsServer();
  }

  //----------------------------- slots for Server --------------------------//

  /**
   * @brief Handle the Client List Item Clicked
   */
  void handleClientListChange(QList<QPair<QHostAddress, quint16>> clients) {
    // Create a list of tuple with Action
    QList<std::tuple<QHostAddress, quint16, Action>> clients_m;

    // Add the clients to the list
    for (auto c : clients) {
      clients_m.append({c.first, c.second, Action::Disconnect});
    }

    // set the client list to the window
    window.setClientList(clients_m);

    // set the host count
    window.setHostCount(s_hostsKey, clients_m.size());
  }

  /**
   * @brief Handle the Server State Change
   */
  void handleServerStateChange(bool isStarted) {
    // infer the status from the server state
    const auto status = isStarted ? Status::Active : Status::Inactive;

    // set the server status
    window.setHostStatus(s_statusKey, status);
  }

  //----------------------------- slots for Client --------------------------//

  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<QPair<QHostAddress, quint16>> servers) {
    // Create a list of tuple with Action
    QList<std::tuple<QHostAddress, quint16, Action>> servers_m;

    // get the connected server
    const auto server = controller.getConnectedServer();

    // get the action for the server
    const auto getAction = [&server](const auto& s) {
      return s == server ? Action::Disconnect : Action::Connect;
    };

    // add the server to the list
    for (auto s : servers) {
      servers_m.append({s.first, s.second, getAction(s)});
    }

    // set the server list to the window
    window.setServerList(servers_m);

    // set the host count
    window.setHostCount(c_hostsKey, servers_m.size());
  }

  /**
   * @brief Handle the server status change
   */
  void handleServerStatusChange(bool status) {
    // infer the status from the server state
    const auto status_m = status ? Status::Connected : Status::Disconnected;

    // set the server status
    window.setHostStatus(c_statusKey, status_m);
  }

  //-------------------------- slots for Tray Icon ---------------------//

  /**
   * @brief Handle the tray icon clicked
   */
  void handleTrayIconClicked() {
    if (this->isVisible()) {
      this->hide();
    } else {
      this->show();
    }
  }

 public:  // constructor
  explicit GuiMain(QWidget* parent = nullptr) : QWidget(parent) {
    // set the window flags to frameless and stay on top
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // Connect the signal and slot for client list change
    const auto signal_cl = &Controller::OnClientListChanged;
    const auto slot_cl = &GuiMain::handleClientListChange;
    connect(&controller, signal_cl, this, slot_cl);

    // Connect the signal and slot for server list change
    const auto signal_sl = &Controller::OnServerListChanged;
    const auto slot_sl = &GuiMain::handleServerListChange;
    connect(&controller, signal_sl, this, slot_sl);

    // Connect the signal and slot for server status change
    const auto signal_ss = &Controller::OnServerStateChanged;
    const auto slot_ss = &GuiMain::handleServerStatusChange;
    connect(&controller, signal_ss, this, slot_ss);

    // Connect the signal and slot for server status change
    const auto signal_sc = &Controller::OnServerStateChanged;
    const auto slot_sc = &GuiMain::handleServerStateChange;
    connect(&controller, signal_sc, this, slot_sc);

    // Connect the signal and slot for host action
    const auto signal_ha = &Window::onHostAction;
    const auto slot_ha = &GuiMain::handleHostAction;
    connect(&window, signal_ha, this, slot_ha);

    // Connect the signal and slot for tab change(client)
    const auto signal_tc = &Window::onTabChanged;
    const auto slot_tc = &GuiMain::handleTabChangeForClient;
    connect(&window, signal_tc, this, slot_tc);

    // Connect the signal and slot for tab change(server)
    const auto signal_ts = &Window::onTabChanged;
    const auto slot_ts = &GuiMain::handleTabChangeForServer;
    connect(&window, signal_ts, this, slot_ts);

    // Connect the signal and slot for tray icon
    const auto signal_ia = &TrayIcon::activated;
    const auto slot_ia = &GuiMain::handleTrayIconClicked;
    connect(&trayIcon, signal_ia, this, slot_ia);

    // set the style sheet for GuiMain
    this->setStyleSheet(style);
  }

  /**
   * @brief Destroy the Shadow object
   */
  virtual ~GuiMain() = default;

  /**
   * @brief Set the Tray Icon object
   */
  void setTrayIcon(QIcon icon) { trayIcon.setIcon(icon); }

  /**
   * @brief Show the tray icon
   */
  void showTrayIcon() { trayIcon.show(); }

  /**
   * @brief Hide the tray icon
   */
  void hideTrayIcon() { trayIcon.hide(); }

  /**
   * @brief Show the window to the user
   */
  void showClipBird() {
    this->showFullScreen();
    window.show();
  }

  /**
   * @brief Hide the window from the user
   */
  void hideClipBird() {
    this->setVisible(false);
    window.hide();
  }

 protected:

  /**
   * @brief Handle the mouse press event
   */
  void mousePressEvent(QMouseEvent* event) override {
    this->hide();
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
