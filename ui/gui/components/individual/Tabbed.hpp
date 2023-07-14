#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QTabWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::individual {
class Tabbed : public QTabWidget {
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Tabbed object
   * with parent as QTabWidget
   * @param parent parent object
   */
  explicit Tabbed(QWidget *parent = nullptr) : QTabWidget(parent) {
    // TODO: set the style sheet
  }

  /**
   * @brief Destroy the Tabbed object
   * with virtual destructor
   */
  ~Tabbed() override = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::individual
