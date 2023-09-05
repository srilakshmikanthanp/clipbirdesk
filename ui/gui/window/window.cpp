// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "window.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Gui Main object
 */
Window::Window(QWidget* parent) : QMainWindow(parent) {
  // set mainWindow attributes
  this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // set padding as 20px
  this->setContentsMargins(10, 5, 10, 5);

  // set as Translucent background
  this->setAttribute(Qt::WA_TranslucentBackground);
}

/**
 * @brief Set Content
 */
void Window::setContent(ui::gui::Content* content) {
  this->setCentralWidget(content);
}

/**
 * @brief get Content
 */
ui::gui::Content* Window::getContent() const {
  return dynamic_cast<ui::gui::Content*>(this->centralWidget());
}

/**
 * @brief Override the setVisiblity
 */
void Window::setVisible(bool visible) {
  // if visible is false return
  if (!visible) return QWidget::setVisible(visible);

  // just show the window in left bottom corner
  QWidget::setVisible(visible);

  // get the screen size
  auto screen = QGuiApplication::primaryScreen()->availableGeometry();

  // get the window size
  auto size = this->size();

  // QPoint
  auto point = QPoint();

  // set the x
  point.setX(screen.width() - size.width());

  // set the y
  point.setY(screen.height() - size.height());

  // set the window position
  this->move(point);
}

/**
 * @brief On Show Event
 */
void Window::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  QWidget::setFocus();
  QWidget::activateWindow();
  QWidget::raise();
}

/**
 * @brief Override the focusOutEvent
 *
 * @param event
 */
bool Window::event(QEvent* event) {
  // if event is window deactivation
  if (event->type() == QEvent::WindowDeactivate) {
    this->hide();
  }

  // call the base class event
  return QWidget::event(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
