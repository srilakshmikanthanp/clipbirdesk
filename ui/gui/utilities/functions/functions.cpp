// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "functions.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities{
/**
 * @brief Function used to set the Os level attributes for the widget
 */
void setPlatformAttributes(QWidget *widget) {
#ifdef _WIN32 // if it it windows platform set title bar to preferred theme
  // cast the winId to HWND
  BOOL isDark = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
  HWND hwnd = reinterpret_cast<HWND>(widget->winId());

  // set the title bar to preferred theme
  auto result = DwmSetWindowAttribute(
    hwnd,
    DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
    &isDark,
    sizeof(isDark)
  );

  // suppress -2147024890 Invalid Handle error
  if (result == -2147024890) {
    return;
  }

  // if success return
  if (result == S_OK) {
    return;
  }

  // get the string of the error
  LPSTR errorText = nullptr;

  // get the error string
  FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    nullptr,
    result,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    reinterpret_cast<LPSTR>(&errorText),
    0,
    nullptr
  );

  // log code: string
  qWarning() << result << " : " << errorText;

  // free the error string
  LocalFree(errorText);
#endif
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::utilities
