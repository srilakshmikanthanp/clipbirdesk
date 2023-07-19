// // Copyright (c) 2023 Sri Lakshmi Kanthan P
// //
// // This software is released under the MIT License.
// // https://opensource.org/licenses/MIT

// /**
//  * This File is set with basic statements to start the
//  * app as desired. Later it will be updated with features
//  * such single instance application, etc.
//  */

// // Qt Headers
// #include <QApplication>
// #include <QScreen>
// #include <QSystemTrayIcon>

// // C++ Headers
// #include <csignal>

// // Project Headers
// #include "constants/constants.hpp"
// #include "controller/clipbird/clipbird.hpp"
// #include "ui/gui/traymenu/traymenu.hpp"
// #include "ui/gui/utilities/utilities.hpp"
// #include "ui/gui/window/window.hpp"
// #include "utility/functions/sslcert/sslcert.hpp"

// using namespace srilakshmikanthanp::clipbirdesk;

// /**
//  * @brief main function that starts the application
//  * by ensuring that only one instance of the
//  * application is running
//  *
//  * @param argc Unused
//  * @param argv Unused
//  *
//  * @return int Status code
//  */
// auto main(int argc, char **argv) -> int {
//   // create SingleApplication instance
//   QApplication app(argc, argv);

//   // create the controller
//   auto controller = controller::ClipBird(QApplication::clipboard());

//   // Create the SSL Config
//   auto sslConfig  = utility::functions::getQSslConfiguration();

//   // set Authenticator
//   controller.setAuthenticator(ui::gui::authenticator);

//   // set the ssl config
//   controller.setSSLConfiguration(sslConfig);

//   // create window
//   auto window   = ui::gui::Window(&controller);

//   // // create the tray icon
//   auto trayIcon = QSystemTrayIcon();

//   // create the tray menu
//   auto trayMenu = ui::gui::TrayMenu();

//   // set the window Ratio
//   window.setSizeRatio(constants::getAppWindowRatio());

//   // // set the icon to tray
//   trayIcon.setIcon(QIcon(constants::getAppLogo().c_str()));

//   signal(SIGTERM, [](int sig) { qApp->quit(); });
//   signal(SIGABRT, [](int sig) { qApp->quit(); });
//   signal(SIGINT, [](int sig) { qApp->quit(); });

//   // set the tray menu
//   trayIcon.setContextMenu(&trayMenu);

//   // using some classes
//   using ui::gui::Window;

//   // set activated action
//   QObject::connect(&trayIcon, &QSystemTrayIcon::activated, &window, &Window::show);

//   // // show the tray icon
//   trayIcon.show();

//   // return the status code of the app
//   return app.exec();
// }

#include <iostream>
#include <stdexcept>
#include <dns_sd.h>

// Callback function for service discovery
void DNSSD_API BrowseCallback(DNSServiceRef serviceRef, DNSServiceFlags flags, uint32_t interfaceIndex,
                              DNSServiceErrorType errorCode, const char* serviceName, const char* regtype,
                              const char* replyDomain, void* context)
{
    if (errorCode == kDNSServiceErr_NoError)
    {
        std::cout << "Service discovered: " << serviceName << " (" << regtype << ")" << std::endl;
    }
    else
    {
        throw std::runtime_error("Service discovery error");
    }
}

int main()
{
    DNSServiceRef serviceRef;
    DNSServiceErrorType error = DNSServiceBrowse(&serviceRef, 0, 0, "_my-service._tcp", nullptr, BrowseCallback, nullptr);

    if (error != kDNSServiceErr_NoError)
    {
        throw std::runtime_error("Failed to browse for services");
    }

    // Run the service discovery indefinitely
    DNSServiceProcessResult(serviceRef);

    // Clean up
    DNSServiceRefDeallocate(serviceRef);

    return 0;
}
