// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "notify.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
Notify::Notify(QWidget *parent) : QDialog(parent) {
  // Set the errorMessage label to center
  this->errorMessage->setAlignment(Qt::AlignCenter);

  // Create a layout
  auto layout = new QVBoxLayout(this);

  // Add the errorMessage label to the layout
  layout->addWidget(this->errorMessage);

  // Set the layout
  this->setLayout(layout);
}

/**
 * @brief set the error message
 */
void Notify::setMessage(const QString &error) {
  this->errorMessage->setText(error);
  this->update();
}

/**
 * @brief get the error message
 */
QString Notify::getMessage() const {
  return this->errorMessage->text();
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
