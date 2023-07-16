// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "circle.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
/**
 * @brief Construct a new Circle object
 * with parent as QWidget
 * @param parent parent object
 */
explicit Circle::Circle(QWidget *parent = nullptr) : QWidget(parent) {
  setStyleSheet(style);
}

/**
 * @brief set the Radius of the circle
 */
void Circle::setRadius(int radius) { this->radius = radius; }

/**
 * @brief set the Color of the circle
 */
void Circle::setColor(const QColor &color) { this->color = color; }

/**
 * @brief get the Radius of the circle
 */
int Circle::getRadius() const { return radius; }

/**
 * @brief get the Color of the circle
 */
QColor Circle::getColor() const { return color; }

/**
 * @brief paint event
 */
void Circle::paintEvent(QPaintEvent *event) override {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(color);
  painter.drawEllipse(0, 0, radius, radius);
}

}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
