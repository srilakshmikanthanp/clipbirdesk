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

  // set as Translucent background
  this->setAttribute(Qt::WA_TranslucentBackground);

  // set padding as 20px
  this->setContentsMargins(20, 20, 20, 20);
}

/**
 * @brief Override the setVisiblity
 */
void Window::setVisible(bool visible) {
  // if visible is false return
  if (!visible) return QWidget::setVisible(visible);

  // screen center
  auto center = QGuiApplication::primaryScreen()->geometry().center();

  // set the window to center
  QWidget::move(center - QWidget::rect().center());

  // show the Window
  QWidget::setVisible(visible);
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
