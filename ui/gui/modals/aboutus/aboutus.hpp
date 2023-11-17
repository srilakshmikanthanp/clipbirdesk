#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDesktopServices>
#include <QEvent>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStyleHints>

// project header
#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class AboutUs : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(AboutUs)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable (With Text Info)

  // widgets
  QPushButton *donate = new QPushButton(this);
  QPushButton *web    = new QPushButton(this);
  QLabel *logo        = new QLabel(this);
  QLabel *version     = new QLabel(this);
  QPushButton *bug    = new QPushButton(this);

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:

 /**
  * @brief Construct a new Abstract object
  *
  * @param parent
  */
  explicit AboutUs(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~AboutUs() = default;

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
