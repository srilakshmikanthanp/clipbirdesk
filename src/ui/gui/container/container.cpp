// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "container.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
/**
 * @brief Construct a new Gui Main object
 */
Container::Container(QWidget* parent) : QWidget(parent) {
  // set mainWindow attributes
  this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  // set as Translucent background
  this->setAttribute(Qt::WA_TranslucentBackground);
}

/**
 * @brief Set Content
 */
void Container::setContent(ui::gui::Clipbird* content) {
  // create layout for the main window
  auto main = new QVBoxLayout(this);

  // add the wrapper to the main layout
  main->addWidget(this->content = content);

  // shadow effect
  auto shadow = new QGraphicsDropShadowEffect(this);

  // set the blur radius
  shadow->setBlurRadius(50);

  // set the color
  shadow->setColor(QColor(0, 0, 0, 30));

  // set the offset
  shadow->setOffset(0, 0);

  // set the shadow effect
  content->setGraphicsEffect(shadow);

  // content margin
  main->setContentsMargins(5, 5, 2, 2);

  // set the main layout to the window
  this->setLayout(main);
}

/**
 * @brief get Content
 */
ui::gui::Clipbird* Container::getContent() const {
  return this->content;
}

/**
 * @brief Override the setVisiblity
 */
void Container::setVisible(bool visible) {
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
 * @brief Override the focusOutEvent
 *
 * @param event
 */
bool Container::event(QEvent* event) {
  // if event is window deactivation
  if (event->type() == QEvent::WindowDeactivate) {
    this->hide();
  }

  // call the base class event
  return QWidget::event(event);
}

/**
 * @brief On Show Event
 */
void Container::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  QWidget::setFocus();
  QWidget::activateWindow();
  QWidget::raise();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
