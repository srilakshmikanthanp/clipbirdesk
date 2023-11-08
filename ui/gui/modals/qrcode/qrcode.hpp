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
#include "ui/gui/modals/modal/modal.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class QrCode : public Modal {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(QrCode)

 private:  // just for Qt

  Q_OBJECT

 private:  // private member variable

  // create label
  components::QrCode *qrcode = new components::QrCode(this);

  // create label
  QLabel *port = new QLabel(this);

 public:

 /**
  * @brief Construct a new Modal object
  *
  * @param parent
  */
  explicit QrCode(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~QrCode() = default;

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
