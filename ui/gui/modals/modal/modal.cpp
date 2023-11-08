// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "modal.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Set the Up Title Bar of object Either dark or light mode
 *
 * @param scheme
 */
void Modal::setUpTitleBar(Qt::ColorScheme scheme) {
// Gonna set up title bar of window using native methods :)
#ifdef _WIN32
  // check the color scheme
  BOOL isDark = scheme == Qt::ColorScheme::Dark;
  auto winID  = this->winId();
  auto hwnd   = reinterpret_cast<HWND>(winID);

  // set the title bar
  auto result = DwmSetWindowAttribute(
    hwnd,
    DWMWA_USE_IMMERSIVE_DARK_MODE,
    &isDark,
    sizeof(isDark)
  );

  // is success
  if (result == S_OK) {
    return;
  }

  // get reason for failure as string
  auto error = QString::fromUtf8(
    std::system_category().message(result).c_str()
  );

  // log
  qDebug() << "DwmSetWindowAttribute :" << error;
#endif
}

/**
 * @brief Construct a new Modal object
 *
 * @param parent
 */
Modal::Modal(QWidget * parent) : QDialog(parent) {
  // on window color scheme change
  const auto styleHints = QGuiApplication::styleHints();
  const auto slot = &Modal::setUpTitleBar;
  const auto signal = &QStyleHints::colorSchemeChanged;
  QObject::connect(styleHints, signal, this, slot);

  // Set the title bar initial
  setUpTitleBar(styleHints->colorScheme());
}
} // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
