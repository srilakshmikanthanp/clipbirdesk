// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "button.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Button object
 * with parent as QButton
 * @param parent parent object
 */
Button::Button(QWidget *parent) : QPushButton(parent) {
  setStyleSheet(style);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
