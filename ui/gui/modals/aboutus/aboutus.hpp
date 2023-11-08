#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDialog>
#include <QStyleHints>

// project header
#include "ui/gui/modals/modal/modal.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class AboutUs : public Modal {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(AboutUs)

 private:  // just for Qt

  Q_OBJECT

 public:

 /**
  * @brief Construct a new Modal object
  *
  * @param parent
  */
  explicit AboutUs(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~AboutUs() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
