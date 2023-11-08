#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QDialog>
#include <QEvent>
#include <QGuiApplication>
#include <QStyleHints>

// Windows headers
#if defined(_WIN32)
#include <dwmapi.h>
#endif

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Modal : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Modal)

 private:  // just for Qt

  Q_OBJECT

 private:  // private member Function

 /**
  * @brief Set the Up Title Bar of object
  *
  * @param scheme
  */
  void setUpTitleBar(Qt::ColorScheme scheme);

 public:

 /**
  * @brief Construct a new Modal object
  *
  * @param parent
  */
  explicit Modal(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Modal() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
