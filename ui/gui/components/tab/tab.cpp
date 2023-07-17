// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "tab.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Tab object
 * with parent as QTabWidget
 * @param parent parent object
 */
Tab::Tab(QWidget *parent) : QTabWidget(parent) {
  setStyleSheet(style);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
