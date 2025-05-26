#include "eventfilter.hpp"
#include "QMessageBox"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Construct a new Clipbird Native Event Filter object
 */
NativeEventFilter::NativeEventFilter(controller::ClipBird *controller) : controller(controller) {
#if defined(__linux__)
  this->registerPowerManagementListener();
#endif
}

bool NativeEventFilter::nativeEventFilter(
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
bool NativeEventFilter::acquireInhibitLock() {
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

void NativeEventFilter::releaseInhibitLock() {
  if (inhibitLock) {
    inhibitLock->close();
    delete inhibitLock;
    inhibitLock = nullptr;
  }
}

void NativeEventFilter::registerPowerManagementListener() {
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

void NativeEventFilter::PrepareForSleep(bool suspending) {
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
void NativeEventFilter::handleWindowsGenericMessage(MSG *msg) {
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

void NativeEventFilter::handleSleepEvent() {
  switch (controller->getHostType()) {
  case types::enums::HostType::CLIENT:
    controller->disposeClient();
    break;

  case types::enums::HostType::SERVER:
    controller->disposeServer();
    break;
  }
}

void NativeEventFilter::handleWakeUpEvent() {
  if (controller->isLastlyHostIsServer()) {
    controller->setCurrentHostAsServer();
  } else {
    controller->setCurrentHostAsClient();
  }
}

/**
 * @brief Destroy the Clipbird Native Event Filter object
 */
NativeEventFilter::~NativeEventFilter() = default;

/**
 * @brief Construct a new Clipbird Application Event Filter object
 */
AppEventFilter::AppEventFilter(controller::ClipBird *controller) : controller(controller) {
  // Nothing to do
}

bool AppEventFilter::eventFilter(QObject *o, QEvent *e) {
  if (e->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(e);
    QWidget *window     = dynamic_cast<QWidget *>(o);
    if (keyEvent && window && keyEvent->key() == Qt::Key_Escape) {
      handleEscKeyPressEvent(window);
    }
  }

  if (e->type() == QEvent::WindowActivate) {
    QWidget *window = dynamic_cast<QWidget *>(o);
    if (window) {
      handleWindowShownEvent(window);
    }
  }

  return QObject::eventFilter(o, e);
}

void AppEventFilter::handleWindowShownEvent(QWidget *window) {
  if (!(window->windowFlags() & Qt::FramelessWindowHint)) {
    ui::gui::utilities::setPlatformAttributes(window);
  }
}

void AppEventFilter::handleEscKeyPressEvent(QWidget *window) {
  if (window->isWindow()) {
    window->hide();
  }
}

/**
 * @brief Destroy the Clipbird Application Event Filter object
 */
AppEventFilter::~AppEventFilter() = default;
}  // namespace srilakshmikanthanp::clipbirdesk
