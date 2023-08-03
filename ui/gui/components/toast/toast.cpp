// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "toast.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Toast object
 * with parent as QWidget
 * @param parent parent object
 */
Toast::Toast(QWidget* parent) : QWidget(parent) {
  // set the Flags and attributes
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_DeleteOnClose);
  setAttribute(Qt::WA_ShowWithoutActivating);

  // set the layout
  auto layout = new QHBoxLayout();
  label->setAlignment(Qt::AlignCenter);
  layout->addWidget(label);
  this->setLayout(layout);

  // set padding
  label->setContentsMargins(10, 10, 10, 10);

  // set the timer
  connect(timer, &QTimer::timeout, this, &Toast::close);
}

/**
 * @brief set the timeout
 * @param val timeout
 */
void Toast::setTimeout(quint32 val) {
  ;
  timer->setInterval((timeout = val));
}

/**
 * @brief get the timeout
 * @return quint32 value
 */
quint32 Toast::getTimeout() const {
  return timeout;
}

/**
 * @brief set the Toast
 * @param val Toast
 */
void Toast::setToast(const QString& val) {
  // set the text to label
  label->setText(val);

  // set the size hint
  auto size = label->sizeHint();

  // set the height and width
  size.setHeight(size.height() + 20);
  size.setWidth(size.width() + 20);

  // set the fixed size
  this->setFixedSize(size);
}

/**
 * @brief get the Toast
 * @return QString value
 */
QString Toast::getToast() const {
  return label->text();
}

/**
 * @brief Override the setVisible
 * @param visible visible
 */
void Toast::setVisible(bool visible) {
  if (visible) {
    // show on bottom center
    auto screen = QApplication::primaryScreen();
    auto size   = screen->size();
    auto rect   = geometry();
    auto point  = QPoint();

    // set the geometry
    point.setX(size.width() / 2 - rect.width() / 2);
    point.setY(size.height() - rect.height() - 50);

    // move to center
    rect.moveTopLeft(point);

    // set the geometry
    this->setGeometry(rect);

    // start the timer
    this->timer->start();
  }

  // call the base class
  QWidget::setVisible(visible);
}

/**
 * @brief Override the moveEvent
 * @param event event
 */
void Toast::enterEvent(QEnterEvent* event) {
  this->hide();
}

/**
 * @brief show the Toast
 * @param parent parent
 * @param val Toast
 * @param timeout timeout
 */
void Toast::toast(const QString& val, quint32 timeout, QWidget* parent) {
  auto toast = new Toast(parent);
  toast->setToast(val);
  toast->setTimeout(timeout);
  toast->show();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
