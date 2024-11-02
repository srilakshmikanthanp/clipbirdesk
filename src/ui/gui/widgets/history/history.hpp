#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt header
#include <QDialog>
#include <QPainterPath>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QScrollBar>
#include <QScrollArea>
#include <QStyleHints>
#include <QVBoxLayout>

// Local headers
#include "ui/gui/components/cliphistory/cliphistory.hpp"
#include "ui/gui/components/clipsend/clipsend.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets {
class History : public QWidget {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(History)

 private:  // just for Qt

  Q_OBJECT

 private:

  components::ClipHistory *clipHist = new components::ClipHistory();
  components::ClipSend* clipSend    = new components::ClipSend();

 signals:  // signals

  // called when the clip is copied
  void onClipSelected(int index);

  // called when the clip is deleted
  void onClipDelete(int index);

  // on send
  void onClipSend();

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
  explicit History(QWidget * parent = nullptr);

  /**
   * @brief Destroy the Status object
   */
  virtual ~History() = default;

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
  QList<QVector<QPair<QString, QByteArray>>> getHistory() const;

  /**
   * @brief override set visible
   */
  void setVisible(bool visible) override;

 protected:  // Member Functions

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::widgets
