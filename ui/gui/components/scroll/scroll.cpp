// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "scroll.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
Scroll::Scroll(QWidget *parent) : QScrollArea(parent) {
  this->setStyleSheet(style);
}
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
