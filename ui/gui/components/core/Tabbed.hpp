#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QTabWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::core {
class Tabbed : public QTabWidget {
 private:          // Constants
  static constexpr const char *const style = R"(
  )";

 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Tabbed object
   * with parent as QTabWidget
   * @param parent parent object
   */
  explicit Tabbed(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Tabbed object
   * with virtual destructor
   */
  ~Tabbed() override = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::individual
