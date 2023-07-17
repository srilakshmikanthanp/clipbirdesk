// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "twin.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Twin object
 * with parent as QWidget
 * @param parent parent object
 */
Twin::Twin(QWidget* parent) : QWidget(parent) {
  this->setStyleSheet(style);
}

/**
 * @brief Set as a pair
 *
 * @param key key
 * @param val value
 */
void Twin::set(QWidget* key, QWidget* val) {
  // create a layout to align the widgets
  auto root = new QFormLayout(this);

  // save the widgets
  this->key = key;
  this->val = val;

  // add the widgets to the layout
  root->addRow(key, val);

  // align the widgets left to right
  root->setLabelAlignment(Qt::AlignLeft);
  root->setFormAlignment(Qt::AlignLeft);

  // set the layout to the widget
  this->setLayout(root);
}

/**
 * @brief get the key and value as pair
 *
 * @return std::pair<QString, QString> key and value
 */
QPair<QWidget*, QWidget*> Twin::get() const {
  return {this->key, this->val};
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
