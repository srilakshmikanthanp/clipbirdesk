#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDialog>
#include <QStyleHints>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class History : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(History)

 private:  // just for Qt

  Q_OBJECT

 public:

 /**
  * @brief Construct a new Abstract object
  *
  * @param parent
  */
  explicit History(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~History() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
