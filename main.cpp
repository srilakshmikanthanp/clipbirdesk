// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt Headers
#include <QApplication>
#include <SingleApplication>

// C++ Headers

// Project Headers
#include "controller/Controller.hpp"
#include "ui/gui/Clipbird.hpp"

#include "utility/functions/sslcert.hpp"

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
  SingleApplication app(argc, argv);
  return app.exec();
}
