#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header
#include <QDialog>
#include <QGuiApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStyleHints>
#include <QVBoxLayout>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {

class SignIn : public QDialog {
 private:

  Q_DISABLE_COPY_MOVE(SignIn)

 private:

  Q_OBJECT

 private:

  QLineEdit *editUser       = new QLineEdit(this);
  QLineEdit *editPass       = new QLineEdit(this);
  QLabel *lblError          = new QLabel(this);
  QPushButton *signInBtn    = new QPushButton(this);

signals:
  void onSignIn(const QString &usernameOrEmail, const QString &password);

 public:

  explicit SignIn(QWidget *parent = nullptr);
  virtual ~SignIn() = default;

  void setUser(const QString &);
  QString getUser() const;

  void setPassword(const QString &);
  QString getPassword() const;

  void setError(const std::optional<QString> &);
  void setSigningIn(bool);

  void reset();

 private:

  void setUpLanguage();

 protected:

  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
