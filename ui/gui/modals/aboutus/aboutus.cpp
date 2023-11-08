// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "aboutus.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Modal object
 *
 * @param parent
 */
AboutUs::AboutUs(QWidget* parent) : Modal(parent) {
  // Click Handlers
  auto donateClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppDonatePage().c_str()));
  };

  auto webClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppHomePage().c_str()));
  };

  auto bugClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppIssuePage().c_str()));
  };

  // Images and Icons
  auto logoIco   = new QIcon(":/images/logo.png");
  auto webIco    = new QIcon(":/images/web.png");
  auto bugIco    = new QIcon(":/images/bug.png");
  auto donateIco = new QIcon(":/images/dollar.png");

  // widgets
  auto logo      = new QLabel(this);
  auto version   = new QLabel(this);
  auto web       = new QPushButton(this);
  auto bug       = new QPushButton(this);
  auto donate    = new QPushButton(this);

  // set logo of the app
  logo->setPixmap(logoIco->pixmap(QSize(128, 128)));

  // set version
  version->setText("Version: " + QString::fromStdString(constants::getAppVersion()));

  // set icons
  web->setIcon(*webIco);
  bug->setIcon(*bugIco);
  donate->setIcon(*donateIco);

  // set text
  web->setText("Website");
  bug->setText("Report Bug");
  donate->setText("Donate");

  // Vertical Layout
  auto layoutVer = new QVBoxLayout(this);

  // Horizontal Layout
  auto layoutHor = new QHBoxLayout(this);

  // add logo and version
  layoutVer->addWidget(logo);
  layoutVer->addWidget(version);

  // add web, bug and donate
  layoutHor->addWidget(web);
  layoutHor->addWidget(bug);
  layoutHor->addWidget(donate);

  // add layout to vertical layout
  layoutVer->addLayout(layoutHor);

  // center align the labels
  logo->setAlignment(Qt::AlignCenter);
  version->setAlignment(Qt::AlignCenter);

  // Alignments
  layoutVer->setAlignment(Qt::AlignCenter);
  layoutHor->setAlignment(Qt::AlignCenter);

  // set layout
  setLayout(layoutVer);

  // set Fixed size
  setFixedSize(this->sizeHint());

  // Click Handler for web
  connect(web, &QPushButton::clicked, webClick);

  // Click Handler for bug
  connect(bug, &QPushButton::clicked, bugClick);

  // Click Handler for donate
  connect(donate, &QPushButton::clicked, donateClick);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
