// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "pairview.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new PairView object
 * with parent as QWidget
 * @param parent parent object
 */
PairView::PairView(QWidget* parent = nullptr) : QWidget(parent) {
  this->setStyleSheet(style);
}

/**
 * @brief Set as a pair
 *
 * @param key key
 * @param val value
 */
void PairView::set(QWidget* key, QWidget* val) {
  // create a layout to align the widgets
  auto root = new QFormLayout(this);

  // save the widgets
  this->key = key;
  this->val = val;

  // add the widgets to the layout
  root->addRow(key, val);

  // set the layout to the widget
  this->setLayout(root);
}

/**
 * @brief get the key and value as pair
 *
 * @return std::pair<QString, QString> key and value
 */
QPair<QWidget*, QWidget*> PairView::get() const {
  return {this->key, this->val};
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
