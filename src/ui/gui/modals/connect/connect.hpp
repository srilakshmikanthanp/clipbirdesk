#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header
#include <QDialog>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStyleHints>

// project header
#include "constants/constants.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals {
class Connect : public QDialog {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Connect)

 private:  // just for Qt

  Q_OBJECT

 signals:   // Signals

  void onConnect(const QString &ipv4, const QString &port);

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:

 /**
  * @brief Construct a new Abstract object
  *
  * @param parent
  */
  explicit Connect(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Connect() = default;

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::modals
