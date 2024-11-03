#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Qt headers
#include <QStackedLayout>
#include <QEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QHostAddress>
#include <QHostInfo>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyle>
#include <QStyleOption>

// C++ Headers
#include <tuple>

// local headers
#include "types/device.hpp"
#include "ui/gui/components/hosttile/hosttile.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::components {
class HostList : public QWidget {
 signals:  // Signals
  void onAction(components::HostTile::Value host);

 private:  // just for Qt

  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(HostList)

 public:  // Type alias

  using Action = components::HostTile::Action;

 private:  // Member Variables

  QStackedLayout* stackLayout = new QStackedLayout(this);
  QWidget* verticalWidget     = new QWidget(this);
  QVBoxLayout* verticalLayout = new QVBoxLayout(verticalWidget);

 private:  // Member variable (With Text Info)

  QLabel *label = new QLabel(this);

 private:  // Member Functions

  /**
   * @brief Function used to set up all text in the label, etc..
   */
  void setUpLanguage();

 public:  // Member Functions

  explicit HostList(QWidget* parent = nullptr);

  /**
   * @brief Set the Hosts to the list
   */
  void setHosts(QList<components::HostTile::Value> hosts);

  /**
   * @brief Get the All Hosts from the list
   */
  QList<components::HostTile::Value> getHosts();

  /**
   * @brief Remove all Hosts from the list
   */
  void removeHosts();

  /**
   * @brief Add Device to the list
   */
  void addHost(components::HostTile::Value host);

  /**
   * @brief Remove a Device from the list
   */
  void removeHost(components::HostTile::Value host);

 protected:  // Member Functions

  /**
   * @brief Override teh show event
   */
  void paintEvent(QPaintEvent* event) override;

  /**
   * @brief Override change event
   */
  void changeEvent(QEvent *) override;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::window
