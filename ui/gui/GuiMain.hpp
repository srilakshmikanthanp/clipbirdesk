#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QApplication>
#include <QWidget>

// Project Headers
#include "controller/Controller.hpp"
#include "ui/gui/TrayIcon.hpp"
#include "ui/gui/Window.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
class AppMain : public QWidget {
 private:  // Disable copy constructor and copy assignment operator
  Q_DISABLE_COPY_MOVE(AppMain)

 private:  // typedefs
  using Controller = controller::Controller;
  using Window = ui::gui::Window;
  using TrayIcon = ui::gui::TrayIcon;

 private:  // Member Variables
  Controller controller = Controller(QApplication::clipboard(), this);
  Window window = Window(this);
  TrayIcon trayIcon = TrayIcon(this);

 private:  // just for Qt
  Q_OBJECT

 private:  // private slots
  //--------------------------------- slots for Window ---------------------------------//

  /**
   * @brief handle the host action from the window
   */
  void handleHostAction(Window::Tabs tab, Window::Action a, QString host, QString ip) {

  }

  /**
   * @brief handle the tab change from the window
   */
  void handleTabChange(Window::Tabs tab) {
    if (tab == Window::Tabs::Server) {
      controller.setCurrentHostAsServer();
    } else {
      controller.setCurrentHostAsClient();
    }
  }

  //----------------------------- slots for Server --------------------------//
  /**
   * @brief Handle the Client List Item Clicked
   */
  void handleClientListChange(QList<QSslSocket *> clients) {
    for (auto client : clients) {
      window.addClient(client->peerName(), client->peerAddress().toString());
    }
  }

  //----------------------------- slots for Client --------------------------//
  /**
   * @brief Handle the Server List Item Clicked
   */
  void handleServerListChange(QList<QPair<QHostAddress, quint16>> servers) {
    // get the connected server to set Action
    const auto connectedServer = controller.getConnectedServer();

    // add the server to the list
    for (auto server : servers) {
      // TODO: add the server to the list
    }
  }

 public:   // constructor
  explicit AppMain(QWidget* parent = nullptr) : QWidget(parent) {
    // set the window flags to frameless and stay on top
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // Connect the signal and slot for host action
    const auto signal_a = &Window::onHostAction;
    const auto slot_a = &AppMain::handleHostAction;
    connect(&window, signal_a, this, slot_a);

    // Connect the signal and slot for tab change
    const auto signal_t = &Window::onTabChanged;
    const auto slot_t = &AppMain::handleTabChange;
    connect(&window, signal_t, this, slot_t);
  }

  /**
   * @brief Destroy the Shadow object
   */
  virtual ~AppMain() = default;
};
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui
