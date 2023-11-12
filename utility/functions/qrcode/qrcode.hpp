#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt header
#include <QPainter>
#include <QImage>
#include <QString>

// qrencode header
#include <qrencode.h>

namespace srilakshmikanthanp::clipbirdesk::utility::functions {
/**
 * @brief Get QrCode Image from text
 *
 * @param text text to encode
 * @param color color of the qrcode
 * @param size size of the qrcode
 */
QImage getQrCode(const QString& text, QColor color, QSize size = QSize(300, 300));
}
