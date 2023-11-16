#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities{
/**
 * @brief Function used to set the Os level attributes for the widget
 */
void setPlatformAttributes(QWidget *widget);
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities
