#include "qrcode.hpp"

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get QrCode Image from text
 */
QImage getQrCode(const QString& text, QColor color, QSize size) {
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

  // check if qrcode is null set text as Error
  if (qrcode.get() == nullptr) {
    return QImage();
  }

  // get the width of the qrcode
  auto width = qrcode->width > 0 ? qrcode->width : 1;

  // get the scale factor for the height
  auto sy = size.height() / width;

  // get the scale factor for the width
  auto sx = size.width() / width;

  // create a image
  QImage image(QSize(width * sx, width * sy), QImage::Format_ARGB32);

  // transparent background
  image.fill(QColor(0,0,0,0));

  // create a painter
  QPainter painter(&image);

  // set the pen to no pen
  painter.setPen(Qt::NoPen);

  // set the brush to color
  painter.setBrush(color);

  // draw the qrcode
  for (int y = 0; y < width; y++) {
    for (int x = 0; x < width; x++) {
      if (qrcode->data[y * width + x] & 0x01) {
        painter.drawRect(QRectF(x * sx, y * sy, sx, sy));
      }
    }
  }

  // return the image
  return image;
}
}  // namespace srilakshmikanthanp::clipbirdesk::utility::functions
