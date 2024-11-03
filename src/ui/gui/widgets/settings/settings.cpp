#include "settings.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets {
/**
 * @brief Construct a new Abstract object
 *
 * @param parent
 */
Settings::Settings(QWidget * parent): QWidget(parent) {
  this->easyHideLabel->setText(tr("Close clipbird window's by esc"));
  this->setObjectName("Settings");

  this->label->setObjectName("Heading");
  this->layout->addWidget(this->label, Qt::AlignCenter);
  this->label->setAlignment(Qt::AlignCenter);

  // Easy Hide
  QHBoxLayout* easyHideLayout = new QHBoxLayout();
  easyHideLayout->addWidget(this->easyHideLabel);
  easyHideLayout->addWidget(this->easyHideCheckBox);
  this->layout->addLayout(easyHideLayout);

  this->setLayout(this->layout);
  this->setUpLanguage();

  QObject::connect(
    this->easyHideCheckBox, &QCheckBox::stateChanged,
    this, [this](int state) {
      emit this->onEasyHideChange(state == Qt::Checked);
    }
  );
}

/**
 * @brief Function used to set up all text in the label, etc..
 */
void Settings::setUpLanguage() {
  // Nothing to do
}

/**
 * @brief set the close window on focus out
 */
void Settings::setEasyHide(bool value) {
  this->easyHideCheckBox->setChecked(value);
}

/**
 * @brief get the close window on focus out
 */
bool Settings::getEasyHide() const {
  return this->easyHideCheckBox->isChecked();
}

/**
 * @brief change event
 */
void Settings::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }

  QWidget::changeEvent(event);
}
}
