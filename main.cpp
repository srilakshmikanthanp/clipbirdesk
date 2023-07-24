// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QSystemTrayIcon>
#include <SingleApplication>
#include <QFile>

// C++ Headers
#include <csignal>

// Project Headers
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/traymenu/traymenu.hpp"
#include "ui/gui/utilities/utilities.hpp"
#include "ui/gui/window/window.hpp"
#include "utility/functions/sslcert/sslcert.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Single Application class specialization
 * for ClipBird Application
 */
class ClipbirdApplication : public SingleApplication {
 private:  //  Member Variables and Objects

  controller::ClipBird *controller;
  QSslConfiguration sslConfig;
  ui::gui::Window *window;
  QSystemTrayIcon *trayIcon;
  ui::gui::TrayMenu *trayMenu;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(ClipbirdApplication);

 public:  // Constructors and Destructors

  /**
   * @brief Construct a new Clipbird Application object
   *
   * @param argc argument count   [unused]
   * @param argv argument vector  [unused]
   */
  ClipbirdApplication(int &argc, char **argv) : SingleApplication(argc, argv) {
    // create the objects of the class
    controller = new controller::ClipBird(QApplication::clipboard());
    sslConfig  = utility::functions::getQSslConfiguration();
    window     = new ui::gui::Window(controller);
    trayIcon   = new QSystemTrayIcon();
    trayMenu   = new ui::gui::TrayMenu();

    // set the signal handler for all os
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });

#ifdef Q_OS_LINUX
    // set the signal handler for linux
    signal(SIGKILL, [](int sig) { qApp->quit(); });
#endif

    // set Authenticator
    controller->setAuthenticator(ui::gui::authenticator);

    // set the ssl config
    controller->setSSLConfiguration(sslConfig);

    // set the window Ratio
    window->setSizeRatio(constants::getAppWindowRatio());

    // set the icon to window
    window->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

    // set the icon to tray
    trayIcon->setIcon(QIcon(constants::getAppLogo().c_str()));

    // set the tray menu
    trayIcon->setContextMenu(trayMenu);

    // using some classes
    using ui::gui::Window;

    // set activated action to show the window
    const auto signal_a = &QSystemTrayIcon::activated;
    const auto slot_a   = &Window::show;
    QObject::connect(trayIcon, signal_a, window, slot_a);

    // set the signal for instance Started
    const auto signal_s = &SingleApplication::instanceStarted;
    const auto slot_s   = &Window::show;
    QObject::connect(this, signal_s, window, slot_s);

    // show the tray icon
    trayIcon->show();
  }

  /**
   * @brief Destroy the Clipbird Application
   * Object and it's members
   */
  virtual ~ClipbirdApplication() {
    delete controller;
    delete window;
    delete trayIcon;
  }
};
}  // namespace srilakshmikanthanp::clipbirdesk

/**
 * @brief main function that starts the application
 * by ensuring that only one instance of the
 * application is running
 *
 * @param argc Unused
 * @param argv Unused
 *
 * @return int Status code
 */
auto main(int argc, char **argv) -> int {
  // using ClipbirdApplication class
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;
  using srilakshmikanthanp::clipbirdesk::ClipbirdApplication;

  // make app home directory if not exists
  if (!QDir(getAppHome().c_str()).exists()) {
    QDir().mkdir(getAppHome().c_str());
  }

  // log file to record the logs
  QFile logfile(QString::fromStdString(getAppLogFile()));

  // Set the log file
  Logger::setLogFile(&logfile);

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // create the application
  ClipbirdApplication app(argc, argv);

  // start application and return status code
  return app.exec();
}
