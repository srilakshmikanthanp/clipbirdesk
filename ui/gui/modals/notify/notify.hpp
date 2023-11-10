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

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Notify : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Notify)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QLabel *errorMessage = new QLabel(this);

 public:

 /**
  * @brief Construct a new Abstract object
  *
  * @param parent
  */
  explicit Notify(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Notify() = default;

  /**
   * @brief set the error message
   */
  void setMessage(const QString &);

  /**
   * @brief get the error message
   */
  QString getMessage() const;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
