#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QLabel>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::individual {
class Label : public QLabel {
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Label object
   * with parent as QLabel
   * @param parent parent object
   */
  explicit Label(QWidget *parent = nullptr) : QLabel(parent) {
    // TODO: set style sheet
  }

  /**
   * @brief Destroy the Label object
   * with virtual destructor
   */
  ~Label() override = default;

 private:  // disable copy and move
  Q_DISABLE_COPY_MOVE(Label)
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core
