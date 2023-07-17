// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QApplication>
#include <QScreen>

// C++ Headers

// Project Headers
#include "config/config.hpp"
#include "ui/gui/trayicon/trayicon.hpp"
#include "ui/gui/window/window.hpp"

/**
 * @brief main function that starts the application
 * by ensuring that only one instance of the
 * application is running
 *
 * @param argc Unused
 * @param argv Unused
 *
 * @return int Status code
 */
auto main(int argc, char **argv) -> int {
  // create SingleApplication instance
  QApplication app(argc, argv);

  // using Window
  using srilakshmikanthanp::clipbirdesk::ui::gui::Window;

  // Sample
  auto window = new Window();
  window->show();

  // return the status code of the app
  return app.exec();
}
