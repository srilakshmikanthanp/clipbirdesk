#include "powerhandler.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Construct a new Clipbird Native Event Filter object
 */
PowerHandler::PowerHandler(controller::ClipBird *controller) : controller(controller) {
#if defined(__linux__)
  this->registerPowerManagementListener();
#endif
}

bool PowerHandler::nativeEventFilter(
    const QByteArray &eventType, void *message, qintptr *result
) {
  constexpr const char *WIN_MSG = "windows_generic_MSG";

#if defined(_WIN32) || defined(_WIN64)
  if (eventType == WIN_MSG) {
    handleWindowsGenericMessage(static_cast<MSG *>(message));
  }
#endif

  return false;
}

#if defined(__linux__)
bool PowerHandler::acquireInhibitLock() {
  QDBusInterface iface(service, path, interface, QDBusConnection::systemBus());
  QDBusReply<QDBusUnixFileDescriptor> reply = iface.call(
      "Inhibit",                // method name
      "sleep",                  // mode
      constants::getAppName(),  // app name
      "Preparing for suspend",  // reason
      "delay"                   // flags
  );

  if (reply.isValid()) {
    (inhibitLock = new QFile())->open(reply.value().fileDescriptor(), QIODevice::ReadWrite);
    return true;
  } else {
    qWarning() << "Failed to acquire delay inhibitor:" << reply.error().message();
    return false;
  }
}

void PowerHandler::releaseInhibitLock() {
  if (inhibitLock) {
    inhibitLock->close();
    delete inhibitLock;
    inhibitLock = nullptr;
  }
}

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

void PowerHandler::PrepareForSleep(bool suspending) {
  if (suspending && acquireInhibitLock()) {
    handleSleepEvent();
    releaseInhibitLock();
  }

  if (!suspending) {
    handleWakeUpEvent();
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
