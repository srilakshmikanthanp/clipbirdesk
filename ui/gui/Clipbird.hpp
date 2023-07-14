#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "ui/gui/components/index.hpp"
#include "ui/gui/components/individual/index.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class Clipbird : public QWidget {
 private:  // Member variable
 private:  // just for Qt
  Q_OBJECT

 public:
  /**
   * @brief Construct a new Clipbird object
   * with parent as QWidget
   * @param parent parent object
   */
  explicit Clipbird(QWidget* parent = nullptr) {}
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
