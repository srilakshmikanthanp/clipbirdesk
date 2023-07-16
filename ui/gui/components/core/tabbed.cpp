// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "tabbed.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
/**
 * @brief Construct a new Tabbed object
 * with parent as QTabWidget
 * @param parent parent object
 */
explicit Tabbed::Tabbed(QWidget *parent = nullptr) : QTabWidget(parent) {
  setStyleSheet(style);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
