// Qt Headers
#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QStyleHints>
#include <QSystemTrayIcon>
#include <SingleApplication>

// C++ Headers
#include <csignal>

// Windows Headers
#if defined(_WIN32) || defined(_WIN64)
  #include <wintoastlib.h>
#endif

// Project Headers
#include "application.hpp"
#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "utility/functions/sslcert/sslcert.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Custom event filter for the application that
 * captures window shown event and if the window is decorated
 * apply some attributes to the window
 */
class AppEventFilter : public QObject {
  virtual bool eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::WindowActivate) {
      handleWindowShownEvent(dynamic_cast<QWidget *>(o));
    }

    return QObject::eventFilter(o, e);
  }

  void handleWindowShownEvent(QWidget *window) {
    if (!(window->windowFlags() & Qt::FramelessWindowHint)) {
      ui::gui::utilities::setPlatformAttributes(window);
    }
  }
};

/**
 * @brief Custom event filter for the application that
 * captures Native event and if the device is going to sleep
 * or wake up disconnect
 */
class NativeEventFilter : public QAbstractNativeEventFilter {
 private:

  bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override {
    constexpr const char *WIN_MSG = "windows_generic_MSG";

#if defined(_WIN32) || defined(_WIN64)
    if (eventType == WIN_MSG) {
      handleWindowsGenericMessage(static_cast<MSG *>(message));
    }
#endif

    return false;
  }

#if defined(_WIN32) || defined(_WIN64)
  void handleWindowsGenericMessage(MSG *msg) {
    if (msg->message == WM_POWERBROADCAST) {
      switch (msg->wParam) {
      case PBT_APMRESUMESUSPEND:
        this->handleWakeUpEvent();
        break;

      case PBT_APMSUSPEND:
        this->handleSleepEvent();
        break;
      }
    }
  }
#endif

  void handleSleepEvent() {
    switch (controller->getHostType()) {
    case types::enums::HostType::CLIENT:
      controller->disposeClient();
      break;

    case types::enums::HostType::SERVER:
      controller->disposeServer();
      break;
    }
  }

  void handleWakeUpEvent() {
    if (controller->isLastlyHostIsServer()) {
      controller->setCurrentHostAsServer();
    } else {
      controller->setCurrentHostAsClient();
    }
  }

 private:

  controller::ClipBird *controller;

 public:

  /**
   * @brief Destroy the Clipbird Native Event Filter object
   */
  virtual ~NativeEventFilter() = default;

  /**
   * @brief Construct a new Clipbird Native Event Filter object
   */
  NativeEventFilter(controller::ClipBird *controller) : controller(controller) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk

/**
 * @brief Global Error Handler that helps to log
 */
void globalErrorHandler() {
  try {
    std::rethrow_exception(std::current_exception());
  } catch (const std::exception &e) {
    qCritical() << e.what();
  } catch (...) {
    qCritical() << "Unknown Exception";
  }

  std::abort();
}

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
  // using ClipbirdApplication class from namespace
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogo;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::constants::getAppName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppOrgName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppVersion;
  using srilakshmikanthanp::clipbirdesk::AppEventFilter;
  using srilakshmikanthanp::clipbirdesk::NativeEventFilter;
  using srilakshmikanthanp::clipbirdesk::Application;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

  // std::string to std::wstring
  const auto W = [](std::string str) -> std::wstring {
    return std::wstring(str.begin(), str.end());
  };

  // create the application
  Application app(argc, argv);

  // native event filter
  auto filter = new NativeEventFilter(app.getController());

  // install native event filter
  app.installNativeEventFilter(filter);

  // install event filter
  app.installEventFilter(new AppEventFilter());

#if defined(_WIN32) || defined(_WIN64)
  // create AUMI
  auto appAumi = WinToastLib::WinToast::configureAUMI(
    W(getAppOrgName()), W(getAppName()), W(std::string()), W(getAppVersion())
  );

  // set up wintoast lib
  WinToastLib::WinToast::instance()->setAppName(W(getAppName()));
  WinToastLib::WinToast::instance()->setAppUserModelId(appAumi);

  // initialize
  if (!WinToastLib::WinToast::instance()->initialize()) {
    auto dialog = QMessageBox(nullptr);
    dialog.setText(QObject::tr("Can't Initialize WinToast"));
    dialog.setIcon(QMessageBox::Critical);
    dialog.setWindowTitle(getAppName());
    dialog.setWindowIcon(QIcon(getAppLogo()));
    dialog.exec();
    return EXIT_FAILURE;
  }
#elif __linux__
  // create glib main loop
  auto g_main_loop = g_main_loop_new(nullptr, false);

  // start glib main loop
  auto thread = std::thread([g_main_loop]() {
    g_main_loop_run(g_main_loop);
  });

  // detach
  thread.detach();

  // quiter
  auto quiter = [g_main_loop]() {
    g_main_loop_quit(g_main_loop);
  };

  // set the signal handler
  QObject::connect(&app, &QApplication::aboutToQuit, quiter);

  // initialize libNotify
  if(!notify_init(getAppName())) {
    auto dialog = QMessageBox(nullptr);
    dialog.setText(QObject::tr("Can't Initialize libNotify"));
    dialog.setIcon(QMessageBox::Critical);
    dialog.setWindowTitle(getAppName());
    dialog.setWindowIcon(QIcon(getAppLogo()));
    dialog.exec();
    return EXIT_FAILURE;
  }

  // uninitializer
  auto uniniter = []() {
    notify_uninit();
  };

  // uninitialize
  QObject::connect(&app, &QApplication::aboutToQuit, uniniter);
#endif

  // Home Directory of the application
  auto path = QString::fromStdString(getAppHome());

  // make app home directory if not exists
  if (!QDir(path).exists() && !QDir().mkdir(path)) {
    QMessageBox::critical(nullptr, "Error", "Can't Create App Home");
    return EXIT_FAILURE;
  }

  // set proxy for the application (No Proxy)
  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

#ifdef NDEBUG
  // log file to record the logs
  QFile logfile(QString::fromStdString(getAppLogFile()));

  // open the log file
  logfile.open(QIODevice::WriteOnly | QIODevice::Append);

  // open QStream to write the log file
  QTextStream logstream(&logfile);

  // Set the log file
  Logger::setLogStream(&logstream);
#else
  // open QStream to write the log file
  QTextStream logstream(stdout);

  // Set the log file as std::cout
  Logger::setLogStream(&logstream);
#endif

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // Set the global error handler
  std::set_terminate(globalErrorHandler);

  // start application and return status code
  return app.exec();
}
