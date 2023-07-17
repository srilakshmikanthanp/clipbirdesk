#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QTabWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Tab : public QTabWidget {
 private:          // Constants
  static constexpr const char *const style = R"(
  )";

 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Tab object
   * with parent as QTabWidget
   * @param parent parent object
   */
  explicit Tab(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Tab object
   * with virtual destructor
   */
  ~Tab() override = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components::individual
