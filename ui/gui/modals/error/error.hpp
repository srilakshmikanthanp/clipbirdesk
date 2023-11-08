#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDialog>
#include <QLabel>
#include <QStyleHints>
#include <QVBoxLayout>

// project header
#include "ui/gui/modals/modal/modal.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Error : public Modal {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Error)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QLabel *errorMessage = new QLabel(this);

 public:

 /**
  * @brief Construct a new Modal object
  *
  * @param parent
  */
  explicit Error(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Error() = default;

  /**
   * @brief set the error message
   */
  void setErrorMessage(const QString &);

  /**
   * @brief get the error message
   */
  QString getErrorMessage() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
