#pragma once

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
#include "syncing/lan/controller/lan_controller.hpp"
#include "ui/gui/utilities/functions/functions.hpp"

namespace srilakshmikanthanp::clipbirdesk {
/**
 * @brief Custom event filter for the application that
 * captures Native event and if the device is going to sleep
 * or wake up disconnect
 */
class PowerHandler : public QObject, public QAbstractNativeEventFilter {
 private:
  Q_OBJECT

#if defined(__linux__)
 private:
  QString service = "org.freedesktop.login1";
  QString path = "/org/freedesktop/login1";
  QString interface = "org.freedesktop.login1.Manager";
  std::unique_ptr<QFile> inhibitLock;

 private:
  void registerPowerManagementListener();
  bool acquireInhibitLock();
  void releaseInhibitLock();
  void handleLinuxSuspendEvent();
  void handleLinuxResumeEvent();

public slots:
  void PrepareForSleep(bool suspending);
#endif

#if defined(_WIN32) || defined(_WIN64)
 private:
  void handleWindowsGenericMessage(MSG *msg);
#endif

 private:
  bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

 signals:
  void OnSleepEvent();
  void OnWakeUpEvent();

 private:
  void handleSleepEvent();
  void handleWakeUpEvent();

 public:

  PowerHandler(QObject *parent = nullptr);
  virtual ~PowerHandler();
};
}  // namespace srilakshmikanthanp::clipbirdesk
