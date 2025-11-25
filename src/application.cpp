// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "application.hpp"
#include "ui/gui/binding/image/clipbird_qml_image_provider.hpp"

namespace srilakshmikanthanp::clipbirdesk {
void Application::handleOpenApplication() {
  bool isVisible = engine.rootObjects().first()->property("visible").toBool();
  engine.rootObjects().first()->setProperty("visible", !isVisible);
}

void Application::handleExit() {
  this->quit();
}

Application::Application(int &argc, char **argv) : SingleApplication(argc, argv) {
  trayIcon.setIcon(QIcon(constants::getAppLogo()));
  trayIcon.setContextMenu(&trayMenu);
  trayIcon.setToolTip(constants::getAppName());
  trayIcon.show();

  engine.addImageProvider("clipbird", new ui::gui::qml::ClipbirdQmlImageProvider());
  engine.loadFromModule("Clipbird.Qml", "Main");

  QObject::connect(
    &trayMenu,
    &ui::gui::TrayMenu::OnOpenAppClicked,
    this,
    &Application::handleOpenApplication
  );

  QObject::connect(
    &trayMenu,
    &ui::gui::TrayMenu::OnExitClicked,
    this,
    &Application::handleExit
  );
}

Application::~Application() {}
}
