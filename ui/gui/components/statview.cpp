// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "statview.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new StatView object
 * with parent as QWidget
 * @param parent parent object
 */
StatView::StatView(QWidget* parent = nullptr) : QWidget(parent) {
  // create a layout to align the widgets
  auto root = new QHBoxLayout(this);

  // add the widgets to the layout
  root->addWidget(circle);
  root->addWidget(status);

  // set the layout to the widget
  this->setLayout(root);

  // set the Style to the widget
  this->setStyleSheet(style);
}

/**
 * @brief set the StatView
 * @param val StatView
 */
void StatView::setStatus(Value val) {
  // set the text of the status
  switch (val) {
    case Value::Disconnected:
      status->setText("Disconnected");
      circle->setColor(Qt::red);
      break;
    case Value::Connected:
      status->setText("Connected");
      circle->setColor(Qt::green);
      break;
    case Value::Active:
      status->setText("Active");
      circle->setColor(Qt::green);
      break;
    case Value::Inactive:
      status->setText("Inactive");
      circle->setColor(Qt::red);
      break;
  }
}

/**
 * @brief get the StatView
 * @return QString value
 */
StatView::Value StatView::getStatus() const {
  return value;
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
