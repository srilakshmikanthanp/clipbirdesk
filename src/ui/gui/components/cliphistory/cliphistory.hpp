#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QStyle>
#include <QPushButton>
#include <QStackedLayout>
#include <QStyleOption>

#include "constants/constants.hpp"
#include "ui/gui/components/cliptile/cliptile.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class ClipHistory : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(ClipHistory)

 private:  // just for Qt

  Q_OBJECT

 private:  // Member variable

  QStackedLayout* stackLayout = new QStackedLayout(this);
  QWidget* verticalWidget     = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);
  QList<ClipTile*> list       = QList<ClipTile*>();
  QList<QVector<QPair<QString, QByteArray>>> history;

 private:  // Member variable (With Text Info)

  QLabel *label = new QLabel(this);

 signals:  // signals

  // called when the clip is copied
  void onClipSelected(int index);

  // called when the clip is deleted
  void onClipDelete(int index);

 private:  // Member Functions

  /**
   * @brief on clipDelete Impl
   */
  void onClipDeleteImpl(int idx);

  /**
   * @brief on clipCopy Impl
   */
  void onClipCopyImpl(int idx);

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:  // Constructor and Destructor

  /**
   * @brief Construct a new Clip Hist object
   *
   * @param parent
   */
  ClipHistory(QWidget *parent = nullptr);

  /**
   * @brief Destroy the Clip Hist object
   */
  ~ClipHistory();

 public:  // public member function

  /**
   * @brief Set the History
   */
  void setHistory(const QList<QVector<QPair<QString, QByteArray>>> &);

  /**
   * @brief Clear the History
   */
  void clearHistory();

  /**
   * @brief get the History
   */
  QList<QVector<QPair<QString, QByteArray>>> getHistory();

 protected:  // protected member function

  // paint event
  void paintEvent(QPaintEvent *) override;

  // change event
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::components
