#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <qrencode.h>

#include <QPainter>
#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class QrCode : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(QrCode)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QColor color = Qt::black;
  QString text;

 public:

  /**
   * @brief Construct a new Status object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit QrCode(QWidget* parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~QrCode() = default;

  /**
   * @brief set the Text to be encoded
   */
  void setText(const QString& text);

  /**
   * @brief get the Text to be encoded
   */
  QString getText() const;

  /**
   * @brief set Color of the QrCode
   */
  void setColor(const QColor& color);

  /**
   * @brief get Color of the QrCode
   */
  QColor getColor() const;

 protected:

  /**
   * @brief paint event
   * @param event paint event
   */
  void paintEvent(QPaintEvent* event) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
