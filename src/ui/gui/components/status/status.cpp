// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "status.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Status object
 * with parent as QWidget
 * @param parent parent object
 */
Status::Status(QWidget* parent) : QLabel(parent) {
  this->setObjectName("Status");
  this->setUpLanguage();
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Status::setUpLanguage() {
  // Nothing to do
}

/**
 * @brief set the Status
 * @param val Status
 */
void Status::setStatus(Value val) {
  switch ((this->value = val))
  {
  case Value::Disconnected:
  case Value::Inactive:
    this->setStyleSheet("color: gray");
    break;
  case Value::Connected:
  case Value::Active:
    this->setStyleSheet(QString());
    break;
  }
}

/**
 * @brief get the Status
 * @return QString value
 */
Status::Value Status::getStatus() const {
  return value;
}

/**
 * @brief Set Both at one time
 */
void Status::set(const QString& text, Value val) {
  this->setText(text); this->setStatus(val);
}

/**
 * @brief Get Both at one time
 */
QPair<QString, Status::Value> Status::get() const {
  return {this->text(), this->value};
}

/**
 * @brief change event
 */
void Status::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
