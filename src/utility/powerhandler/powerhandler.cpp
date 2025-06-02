#include "powerhandler.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Construct a new Clipbird Native Event Filter object
 */
PowerHandler::PowerHandler(controller::ClipBird *controller) : controller(controller) {
#if defined(__linux__)
  this->registerPowerManagementListener();
  this->acquireInhibitLock();
#endif
}

bool PowerHandler::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
  constexpr const char *WIN_MSG = "windows_generic_MSG";

#if defined(_WIN32) || defined(_WIN64)
  if (eventType == WIN_MSG) {
    handleWindowsGenericMessage(static_cast<MSG *>(message));
  }
#endif

  return false;
}

#if defined(__linux__)
void PowerHandler::registerPowerManagementListener() {
  QDBusConnection systemBus = QDBusConnection::systemBus();

  if (!systemBus.isConnected()) {
    qWarning() << "Failed to connect to system D-Bus";
    return;
  }

  bool success = systemBus.connect(
      service,                     // service
      path,                        // path
      interface,                   // interface
      "PrepareForSleep",           // signal name
      this,                        // receiver
      SLOT(PrepareForSleep(bool))  // method name
  );

  if (!success) {
    qWarning() << "Failed to connect to PrepareForSleep signal";
  }
}

bool PowerHandler::acquireInhibitLock() {
  QDBusInterface iface(service, path, interface, QDBusConnection::systemBus());
  QDBusReply<QDBusUnixFileDescriptor> reply = iface.call(
      "Inhibit",                // method name
      "sleep",                  // mode
      constants::getAppName(),  // app name
      "Preparing for suspend",  // reason
      "delay"                   // flags
  );

  if (!reply.isValid()) {
    qWarning() << "Failed to acquire delay inhibitor:" << reply.error().message();
    return false;
  }

  inhibitLock = std::make_unique<QFile>();

  if (!inhibitLock->open(reply.value().fileDescriptor(), QIODevice::ReadWrite)) {
    qWarning() << "Failed to open inhibit lock file descriptor";
    return false;
  } else {
    qInfo() << "Inhibit lock acquired successfully";
    return true;
  }
}

void PowerHandler::releaseInhibitLock() {
  if (inhibitLock && inhibitLock->isOpen()) {
    inhibitLock->close();
    inhibitLock.reset();
    qInfo() << "Block inhibitor released.";
  } else {
    qWarning() << "No inhibit lock to release.";
  }
}

void PowerHandler::handleLinuxSuspendEvent() {
  if (inhibitLock && inhibitLock->isOpen()) {
    handleSleepEvent();
    releaseInhibitLock();
  }
}

void PowerHandler::handleLinuxResumeEvent() {
  handleWakeUpEvent();
  acquireInhibitLock();
}

void PowerHandler::PrepareForSleep(bool suspending) {
  if (suspending) {
    qInfo() << "Received suspend signal, preparing for sleep.";
    handleLinuxSuspendEvent();
  } else {
    qInfo() << "Received resume signal, waking up.";
    handleLinuxResumeEvent();
  }
}
#endif

#if defined(_WIN32) || defined(_WIN64)
void PowerHandler::handleWindowsGenericMessage(MSG *msg) {
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

void PowerHandler::handleSleepEvent() {
  switch (controller->getHostType()) {
  case types::enums::HostType::CLIENT:
    controller->disposeClient();
    break;

  case types::enums::HostType::SERVER:
    controller->disposeServer();
    break;
  }
}

void PowerHandler::handleWakeUpEvent() {
  if (controller->isLastlyHostIsServer()) {
    controller->setCurrentHostAsServer();
  } else {
    controller->setCurrentHostAsClient();
  }
}

/**
 * @brief Destroy the Clipbird Native Event Filter object
 */
PowerHandler::~PowerHandler() = default;
}  // namespace srilakshmikanthanp::clipbirdesk
