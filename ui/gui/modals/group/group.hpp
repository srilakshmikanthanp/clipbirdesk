#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QStyleHints>
#include <QVBoxLayout>

// project header
#include "utility/functions/qrcode/qrcode.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Group : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Group)

 private:  // just for Qt

  Q_OBJECT

 private:  // private member variable

  // create label
  QLabel *port = new QLabel(this);

  // create label
  QString text;

  // Image of the qrcode
  QLabel *qrcode = new QLabel();

 private:  // private member functions

  /**
   * @brief Set the Up Qr Code object
   *
   */
  void setUpQrCode(Qt::ColorScheme);

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
  explicit Group(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Group() = default;

  /**
   * @brief set the qr code
   */
  void setQrCode(const QString &);

  /**
   * @brief get the qr code
   */
  QString getQrCode() const;

  /**
   * @brief set the port
   */
  void setPort(const QString &);

  /**
   * @brief get the port
   */
  QString getPort() const;

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
