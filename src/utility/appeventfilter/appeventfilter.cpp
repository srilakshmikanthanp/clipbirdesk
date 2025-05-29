#include "appeventfilter.hpp"

namespace srilakshmikanthanp::clipbirdesk {
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
