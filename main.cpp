// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QApplication>
#include <SingleApplication>

// C++ Headers

// Project Headers
#include "config/config.hpp"

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
  SingleApplication app(argc, argv);

  // check if another instance is running
  if (app.isSecondary()) {
    app.sendMessage(app.arguments().join(' ').toUtf8());
  } else {
    // TODO: add code to start the application
  }

  // return status code
  return app.exec();
}
