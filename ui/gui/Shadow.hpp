#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui {
// used to block the entire screen by using the shadow effect
class Shadow : public QWidget {
 private:  // Disable copy constructor and copy assignment operator
  Q_DISABLE_COPY_MOVE(Shadow)

 private:  // just for Qt
  Q_OBJECT

 public:  // constructor
  explicit Shadow(QWidget* parent = nullptr) : QWidget(parent) {
    // set the window flags to frameless and stay on top
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // TODO set the style sheet
  }

  /**
   * @brief Destroy the Shadow object
   */
  virtual ~Shadow() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui
