// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QDesktopServices>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <SingleApplication>

// C++ Headers
#include <csignal>

// Project Headers
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/guimain/guimain.hpp"
#include "ui/gui/traymenu/traymenu.hpp"
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
  ui::gui::Window *window;
  QSystemTrayIcon *trayIcon;
  ui::gui::TrayMenu *trayMenu;
  ui::gui::GuiMain *guiMain;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(ClipbirdApplication);

  /**
   * @brief On About Clicked
   */
  void onAboutClicked() {
    QDesktopServices::openUrl(QUrl(constants::getAppHomePage().c_str()));
  }

  /**
   * @brief On Issue Clicked
   */
  void onIssueClicked() {
    QDesktopServices::openUrl(QUrl(constants::getAppIssuePage().c_str()));
  }

  /**
   * @brief log the Error
   */
  void logError(QString s) {
    qErrnoWarning(s.toStdString().c_str());
  }

 public:  // Constructors and Destructors

  /**
   * @brief Construct a new Clipbird Application object
   *
   * @param argc argument count   [unused]
   * @param argv argument vector  [unused]
   */
  ClipbirdApplication(int &argc, char **argv) : SingleApplication(argc, argv) {
    // create the objects of the class
    controller = new controller::ClipBird(
        QApplication::clipboard(), utility::functions::getQSslConfiguration()
    );
    window   = new ui::gui::Window(controller);
    trayIcon = new QSystemTrayIcon();
    trayMenu = new ui::gui::TrayMenu();
    guiMain  = new ui::gui::GuiMain();

    // set the signal handler for all os
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });

    // clang-format off
    #ifdef Q_OS_LINUX
    // set the signal handler for linux
    signal(SIGKILL, [](int sig) { qApp->quit(); });
    #endif
    // clang-format on

    // QFile to read the qss file
    QFile qssFile(QString::fromStdString(constants::getAppQSS()));

    // open the qss file
    qssFile.open(QFile::ReadOnly);

    // set the style sheet
    qApp->setStyleSheet(qssFile.readAll());

    // set not to quit on last window closed
    qApp->setQuitOnLastWindowClosed(false);

    // BackDrop Shadow
    auto shadow = new QGraphicsDropShadowEffect();

    // set the shadow Properties
    shadow->setBlurRadius(5);
    shadow->setOffset(5, 5);
    shadow->setColor(QColor(0, 0, 0, 100));

    // set the shadow to window
    window->setGraphicsEffect(shadow);

    // set the window Ratio
    guiMain->setSizeRatio(constants::getAppWindowRatio());

    // set the window
    guiMain->setCentralWidget(window);

    // set the icon to window
    guiMain->setWindowIcon(QIcon(constants::getAppLogo().c_str()));

    // set the icon to tray
    trayIcon->setIcon(QIcon(constants::getAppLogo().c_str()));

    // set the tray menu
    trayIcon->setContextMenu(trayMenu);

    // using some classes
    using ui::gui::Window;

    // set the signal for menus About click
    const auto signal_ac = &ui::gui::TrayMenu::OnAboutClicked;
    const auto slot_ac   = &ClipbirdApplication::onAboutClicked;
    QObject::connect(trayMenu, signal_ac, this, slot_ac);

    // set the signal for menus Issue click
    const auto signal_ic = &ui::gui::TrayMenu::OnIssueClicked;
    const auto slot_ic   = &ClipbirdApplication::onIssueClicked;
    QObject::connect(trayMenu, signal_ic, this, slot_ic);

    // set the signal for menus Quit click
    const auto signal_qc = &ui::gui::TrayMenu::OnExitClicked;
    const auto slot_qc   = [] { qApp->quit(); };
    QObject::connect(trayMenu, signal_qc, this, slot_qc);

    // set activated action to show the window
    const auto signal_at = &QSystemTrayIcon::activated;
    const auto slot_at   = &Window::show;
    QObject::connect(trayIcon, signal_at, guiMain, slot_at);

    // set the signal for instance Started
    const auto signal_is = &SingleApplication::instanceStarted;
    const auto slot_is   = &Window::show;
    QObject::connect(this, signal_is, guiMain, slot_is);

    // set the signal for error occurred
    const auto signal_eo = &controller::ClipBird::OnErrorOccurred;
    const auto slot_eo   = &ClipbirdApplication::logError;
    QObject::connect(controller, signal_eo, this, slot_eo);

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
    delete trayMenu;
    delete guiMain;
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
  using srilakshmikanthanp::clipbirdesk::ClipbirdApplication;
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

  // make app home directory if not exists
  if (!QDir(getAppHome().c_str()).exists()) {
    QDir().mkdir(getAppHome().c_str());
  }

  // log file to record the logs
  QFile logfile(QString::fromStdString(getAppLogFile()));

  // open the log file
  logfile.open(QIODevice::WriteOnly | QIODevice::Append);

  // Set the log file
  Logger::setLogFile(&logfile);

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // create the application
  ClipbirdApplication app(argc, argv);

  // start application and return status code
  return app.exec();
}
