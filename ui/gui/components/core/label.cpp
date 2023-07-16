// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "label.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
/**
 * @brief Construct a new Label object
 * with parent as QLabel
 * @param parent parent object
 */
explicit Label::Label(QWidget *parent = nullptr) : QLabel(parent) {
  setStyleSheet(style);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
