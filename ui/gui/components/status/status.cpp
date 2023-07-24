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

}

/**
 * @brief set the Status
 * @param val Status
 */
void Status::setStatus(Value val) {
  // set the text of the status
  switch (val) {
  case Value::Disconnected:
    this->setStyleSheet("color: red");
    this->setText("Disconnected");
    break;
  case Value::Connected:
    this->setStyleSheet("color: green");
    this->setText("Connected");
    break;
  case Value::Active:
    this->setStyleSheet("color: green");
    this->setText("Active");
    break;
  case Value::Inactive:
    this->setStyleSheet("color: red");
    this->setText("Inactive");
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
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
