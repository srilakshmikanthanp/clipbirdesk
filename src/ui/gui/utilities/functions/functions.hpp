#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QStyleHints>
#include <QGuiApplication>
#include <QStyleHints>
#include <QWidget>

#ifdef _WIN32
#include <dwmapi.h>
#endif

namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities{
/**
 * @brief Function used to set the Os level attributes for the widget
 */
void setPlatformAttributes(QWidget *widget);
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities
