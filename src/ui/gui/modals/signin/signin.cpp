#include "signin.hpp"

#include <QHBoxLayout>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {

SignIn::SignIn(QWidget *parent) : QDialog(parent) {
  this->editPass->setEchoMode(QLineEdit::Password);
  this->editUser->setMinimumWidth(300);
  this->editPass->setMinimumWidth(300);
  this->setUpLanguage();
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(editUser);
  mainLayout->addWidget(editPass);
  mainLayout->addWidget(lblError);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(signInBtn);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);
  connect(signInBtn, &QPushButton::clicked, this, [this]() {
    emit onSignIn(editUser->text(), editPass->text());
  });
}

void SignIn::setUser(const QString &user) {
  editUser->setText(user);
}

QString SignIn::getUser() const {
  return editUser->text();
}

void SignIn::setPassword(const QString &pass) {
  editPass->setText(pass);
}

QString SignIn::getPassword() const {
  return editPass->text();
}

void SignIn::setError(const std::optional<QString> &error) {
  if (error.has_value()) {
    lblError->setText(error.value());
  } else {
    lblError->clear();
  }
}

void SignIn::setSigningIn(bool isSigningIn) {
  if (isSigningIn) {
    editUser->setDisabled(true);
    editPass->setDisabled(true);
    signInBtn->setDisabled(true);
    signInBtn->setText(tr("Signing In..."));
  } else {
    editUser->setDisabled(false);
    editPass->setDisabled(false);
    signInBtn->setDisabled(false);
    signInBtn->setText(tr("Sign In"));
  }
}

void SignIn::reset() {
  signInBtn->setText(tr("Sign In"));
  signInBtn->setDisabled(false);
  editUser->setDisabled(false);
  editPass->setDisabled(false);
  editUser->clear();
  editPass->clear();
  lblError->clear();
}

void SignIn::changeEvent(QEvent *event) {
  if (event->type() == QEvent::LanguageChange) {
    this->setUpLanguage();
  }
  QDialog::changeEvent(event);
}

void SignIn::setUpLanguage() {
  editUser->setPlaceholderText(tr("Username or Email"));
  editPass->setPlaceholderText(tr("Password"));
  signInBtn->setText(tr("Sign In"));
}
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
