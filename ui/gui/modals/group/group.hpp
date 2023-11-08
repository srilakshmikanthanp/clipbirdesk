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
#include "ui/gui/components/qrcode/qrcode.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Group : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Group)

 private:  // just for Qt

  Q_OBJECT

 private:  // private member variable

  // create label
  components::QrCode *qrcode = new components::QrCode(this);

  // create label
  QLabel *port = new QLabel(this);

 private:  // private member functions

  void setUpQrCodeColor(Qt::ColorScheme);

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
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
