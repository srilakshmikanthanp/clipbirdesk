// Qt Headers
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

#ifdef _WIN32
#include <io.h>
#define null_device "NUL"
#else
#include <unistd.h>
#define null_device "/dev/null"
#endif

// Project Headers
#include "application.hpp"
#include "constants/constants.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "utility/appeventfilter/appeventfilter.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "utility/logging/logging.hpp"
#include "utility/powerhandler/powerhandler.hpp"

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
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogo;
  using srilakshmikanthanp::clipbirdesk::constants::getAppName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppOrgName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppVersion;
  using srilakshmikanthanp::clipbirdesk::AppEventFilter;
  using srilakshmikanthanp::clipbirdesk::Application;
  using srilakshmikanthanp::clipbirdesk::PowerHandler;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

  QLoggingCategory::setFilterRules("*.debug=false");

  freopen(null_device, "w", stderr);

  // std::string to std::wstring
  const auto W = [](std::string str) -> std::wstring {
    return std::wstring(str.begin(), str.end());
  };

  // create the application
  Application app(argc, argv);

  // disable proxy for the application
  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

  // install native event filter
  app.installNativeEventFilter(app.getPowerHandler());

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
  auto loop   = g_main_loop_new(nullptr, false);
  auto thread = std::thread([loop]() { g_main_loop_run(loop); });

  // detach
  thread.detach();

  // quiter
  auto quiter = [loop]() { g_main_loop_quit(loop); };

  // set the signal handler
  QObject::connect(&app, &QApplication::aboutToQuit, quiter);

  // initialize libNotify
  if (!notify_init(getAppName())) {
    auto dialog = QMessageBox(nullptr);
    dialog.setText(QObject::tr("Can't Initialize libNotify"));
    dialog.setIcon(QMessageBox::Critical);
    dialog.setWindowTitle(getAppName());
    dialog.setWindowIcon(QIcon(getAppLogo()));
    dialog.exec();
    return EXIT_FAILURE;
  }

  // uninitializer
  auto uniniter = []() { notify_uninit(); };

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
