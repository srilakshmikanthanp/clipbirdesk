// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "qrcode.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
/**
 * @brief Construct a new Qr Code object
 */
QrCode::QrCode(QWidget* parent) : QWidget(parent) {
  // Nothing to do here
}

/**
 * @brief set the Text to be encoded
 */
void QrCode::setText(const QString& text) {
  this->text = text;
  this->update();
}

/**
 * @brief get the Text to be encoded
 */
QString QrCode::getText() const {
  return text;
}

/**
 * @brief set Color of the QrCode
 */
void QrCode::setColor(const QColor& color) {
  this->color = color;
  this->update();
}

/**
 * @brief get Color of the QrCode
 */
QColor QrCode::getColor() const {
  return color;
}

/**
 * @brief paint event
 * @param event paint event
 */
void QrCode::paintEvent(QPaintEvent* event) {
  // create a unique pointer to handle the memory
  using qrcode_ptr = std::unique_ptr<QRcode, decltype(&QRcode_free)>;

  // create a qrcode
  qrcode_ptr qrcode(QRcode_encodeString(
    text.toStdString().c_str(),     // text to encode
    0,                              // version
    QR_ECLEVEL_L,                   // error correction level
    QR_MODE_8,                      // encoding mode
    1                               // case sensitive
  ),
    QRcode_free                     // deleter
  );

  // create a painter
  QPainter painter = QPainter(this);

  // check if qrcode is null set text as Error
  if (qrcode.get() == nullptr) {
    return painter.drawText(this->rect(), Qt::AlignCenter, "Error");
  }

  // set the pen to no pen
  painter.setPen(Qt::NoPen);

  // set the brush to color
  painter.setBrush(color);

  // get the width of the qrcode
  auto width = qrcode->width > 0 ? qrcode->width : 1;

  // get the scale factor for the height
  auto sy = this->height() / width;

  // get the scale factor for the width
  auto sx = this->width() / width;

  // set size hint
  this->setFixedSize(width * sx, width * sy);

  // draw the qrcode
  for (int y = 0; y < width; y++) {
    for (int x = 0; x < width; x++) {
      if (qrcode->data[y * width + x] & 0x01) {
        painter.drawRect(QRectF(x * sx, y * sy, sx, sy));
      }
    }
  }

  // call the base class paint event
  QWidget::paintEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
