
#include <QAbstractNativeEventFilter>
#include <QObject>
#include <QString>
#include <QKeyEvent>
#include <QWidget>

#if defined(__linux__)
#include <QDBusConnection>
#include <QDBusMessage>
#include <QFile>
#include <QDBusInterface>
#include <QDBusUnixFileDescriptor>
#include <QDBusReply>
#include <QDBusConnectionInterface>
#endif

#include "constants/constants.hpp"
#include "controller/clipbird/clipbird.hpp"
#include "ui/gui/utilities/functions/functions.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Custom event filter for the application that
 * captures Native event and if the device is going to sleep
 * or wake up disconnect
 */
class NativeEventFilter : public QObject, public QAbstractNativeEventFilter {
 private:
  Q_OBJECT

#if defined(__linux__)
 private:
  QString service = "org.freedesktop.login1";
  QString path = "/org/freedesktop/login1";
  QString interface = "org.freedesktop.login1.Manager";
  QFile *inhibitLock = nullptr;

 private:
  void registerPowerManagementListener();
  bool acquireInhibitLock();
  void releaseInhibitLock();

public slots:
  void PrepareForSleep(bool suspending);
#endif

 private:

#if defined(_WIN32) || defined(_WIN64)
  void handleWindowsGenericMessage(MSG *msg);
#endif

 private:
  bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
  void handleSleepEvent();
  void handleWakeUpEvent();

 private:

  controller::ClipBird *controller;

 public:

  /**
   * @brief Destroy the Clipbird Native Event Filter object
   */
  virtual ~NativeEventFilter();

  /**
   * @brief Construct a new Clipbird Native Event Filter object
   */
  NativeEventFilter(controller::ClipBird *controller);
};

/**
 * @brief Custom event filter for the application that
 * captures window shown event and if the window is decorated
 * apply some attributes to the window
 */
class AppEventFilter : public QObject {
 private:

  virtual bool eventFilter(QObject *o, QEvent *e) override;
  void handleWindowShownEvent(QWidget *window);
  void handleEscKeyPressEvent(QWidget *window);

 private:

  controller::ClipBird *controller;

 public:

  /**
   * @brief Destroy the Clipbird Application Event Filter object
   */
  virtual ~AppEventFilter();

  /**
   * @brief Construct a new Clipbird Application Event Filter object
   */
  AppEventFilter(controller::ClipBird *controller);
};
}  // namespace srilakshmikanthanp::clipbirdesk
