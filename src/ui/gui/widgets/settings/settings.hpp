#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header
#include <QDialog>
#include <QPainterPath>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets {
class Settings : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(Settings)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member Variables

  QVBoxLayout* layout = new QVBoxLayout();
  QLabel* label = new QLabel(tr("Settings"));
  QLabel* easyHideLabel = new QLabel();
  QCheckBox* easyHideCheckBox = new QCheckBox();

 signals:  // signals

  void closeWindowOnFocusOut(bool value);

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
  explicit Settings(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~Settings() = default;

  /**
   * @brief set the close window on focus out
   */
  void setEasyHide(bool value);

  /**
   * @brief get the close window on focus out
   */
  bool getEasyHide() const;

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets
