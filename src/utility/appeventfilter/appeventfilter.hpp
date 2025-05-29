
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
