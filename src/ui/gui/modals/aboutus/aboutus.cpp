#include "aboutus.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
AboutUs::AboutUs(QWidget* parent) : QDialog(parent) {
  // Click Handlers
  auto donateClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppDonatePage()));
  };

  auto webClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppHomePage()));
  };

  auto bugClick = [=]() {
    QDesktopServices::openUrl(QUrl(constants::getAppIssuePage()));
  };

  // Images and Icons
  auto logoIco   = QIcon(":/images/logo.png");
  auto webIco    = QIcon(":/images/web.png");
  auto bugIco    = QIcon(":/images/bug.png");
  auto donateIco = QIcon(":/images/dollar.png");

  // set logo of the app
  logo->setPixmap(logoIco.pixmap(QSize(128, 128)));

  // set icons
  web->setIcon(webIco);
  donate->setIcon(donateIco);
  bug->setIcon(bugIco);

  // pointer
  web->setCursor(Qt::PointingHandCursor);
  donate->setCursor(Qt::PointingHandCursor);
  bug->setCursor(Qt::PointingHandCursor);

  // Vertical Layout
  auto layoutVer = new QVBoxLayout();

  // Horizontal Layout
  auto layoutHor = new QHBoxLayout();

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
  version->setAlignment(Qt::AlignCenter);
  logo->setAlignment(Qt::AlignCenter);

  // Alignments
  layoutVer->setAlignment(Qt::AlignCenter);
  layoutHor->setAlignment(Qt::AlignCenter);

  // set the layout
  this->setLayout(layoutVer);

  // set up initial language
  this->setUpLanguage();

  // Click Handler for web
  connect(web, &QPushButton::clicked, webClick);

  // Click Handler for donate
  connect(donate, &QPushButton::clicked, donateClick);

  // Click Handler for bug
  connect(bug, &QPushButton::clicked, bugClick);
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void AboutUs::setUpLanguage() {
  // get the app version
  auto v = QString::fromStdString(constants::getAppVersion());

  // set version
  version->setText(QObject::tr("Version: ") + v);

  // set text
  bug->setText(QObject::tr("Report Bug"));
  web->setText(QObject::tr("Website"));
  donate->setText(QObject::tr("Donate"));
}

/**
 * @brief change event
 */
void AboutUs::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
