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
#include "ui/gui/notification/joinrequest/joinrequest.hpp"
#include "ui/gui/utilities/functions/functions.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "utility/appeventfilter/appeventfilter.hpp"
#include "utility/functions/ssl/ssl.hpp"
#include "utility/logging/logging.hpp"
#include "utility/powerhandler/powerhandler.hpp"
#include "common/trust/trusted_servers_factory.hpp"
#include "common/trust/trusted_clients_factory.hpp"
#include "common/types/ssl_config/ssl_config_factory.hpp"

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

auto main(int argc, char **argv) -> int {
  using srilakshmikanthanp::clipbirdesk::constants::getAppHome;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogFile;
  using srilakshmikanthanp::clipbirdesk::constants::getAppLogo;
  using srilakshmikanthanp::clipbirdesk::constants::getAppName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppOrgName;
  using srilakshmikanthanp::clipbirdesk::constants::getAppVersion;
  using srilakshmikanthanp::clipbirdesk::AppEventFilter;
  using srilakshmikanthanp::clipbirdesk::Application;
  using srilakshmikanthanp::clipbirdesk::logging::Logger;

  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

  freopen(null_device, "w", stderr);

  const auto W = [](std::string str) -> std::wstring {
    return std::wstring(str.begin(), str.end());
  };

  Application app(argc, argv);

  app.installEventFilter(new AppEventFilter());
  app.installEventFilter(app.getPowerHandler());

#if defined(_WIN32) || defined(_WIN64)
  auto appAumi = WinToastLib::WinToast::configureAUMI(
      W(getAppOrgName()), W(getAppName()), W(std::string()), W(getAppVersion())
  );

  WinToastLib::WinToast::instance()->setAppName(W(getAppName()));
  WinToastLib::WinToast::instance()->setAppUserModelId(appAumi);

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

  thread.detach();

  auto quiter = [loop]() { g_main_loop_quit(loop); };

  QObject::connect(&app, &QApplication::aboutToQuit, quiter);

  if (!notify_init(getAppName())) {
    auto dialog = QMessageBox(nullptr);
    dialog.setText(QObject::tr("Can't Initialize libNotify"));
    dialog.setIcon(QMessageBox::Critical);
    dialog.setWindowTitle(getAppName());
    dialog.setWindowIcon(QIcon(getAppLogo()));
    dialog.exec();
    return EXIT_FAILURE;
  }

  auto uniniter = []() { notify_uninit(); };

  QObject::connect(&app, &QApplication::aboutToQuit, uniniter);
#endif

  auto path = QString::fromStdString(getAppHome());

  if (!QDir(path).exists() && !QDir().mkdir(path)) {
    QMessageBox::critical(nullptr, "Error", "Can't Create App Home");
    return EXIT_FAILURE;
  }

  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

#ifdef NDEBUG
  QFile logfile(QString::fromStdString(getAppLogFile()));
  logfile.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream logstream(&logfile);
  Logger::setLogStream(&logstream);
#else
  QTextStream logstream(stdout);
  Logger::setLogStream(&logstream);
#endif

  qInstallMessageHandler(Logger::handler);
  qApp->setQuitOnLastWindowClosed(false);
  std::set_terminate(globalErrorHandler);

  return app.exec();
}
