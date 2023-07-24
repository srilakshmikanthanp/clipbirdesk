#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QTabWidget>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Tab : public QTabWidget {
 private:  // Constants

  static constexpr const char *const style = R"(
    QTabWidget {
      background-color: #1f1e2c;
    }

    QTabWidget::pane {
        border-top-color: transparent;
        background-color: #1f1e2c;
    }

    QTabBar::tab {
        color: #ffffff;
        background-color: #1f1e2c;
        border: 0px;
        padding-left: 10px;
        padding-right: 10px;
        padding-top: 5px;
        padding-bottom: 5px;
        margin: 0px 45px;
        font-size: 25px;
        font-family: sans-serif;
        font-weight: 500;
    }

    QTabBar::tab:selected {
      border-bottom-width: 6px;
      border-bottom-color: #0375ef;
    }

    QTabBar::tab:hover {
      border-bottom-width: 5px;
      border-bottom-color: #0375ef;
    }
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
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
