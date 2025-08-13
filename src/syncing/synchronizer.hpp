#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <QObject>

namespace srilakshmikanthanp::clipbirdesk::network::syncing {
class Synchronizer : public QObject {
 private:  // just for Qt

  /// @brief Qt meta object
  Q_OBJECT

 private:  // disable copy and move

  Q_DISABLE_COPY_MOVE(Synchronizer)

 public:  // constructor and destructor

  /**
   * @brief Construct a new Synchronizer object
   *
   * @param parent Parent object
   */
  explicit Synchronizer(QObject* parent = nullptr) : QObject(parent) {}

  /**
   * @brief Destroy the Synchronizer object
   */
  virtual ~Synchronizer();

 public:  // methods

  /** @brief Synchronize the clipboard data
   *
   * This method is responsible for synchronizing the clipboard data
   * between different clients. It will handle the logic to ensure that
   * all clients have the same clipboard content.
   */
  virtual void synchronize(QVector<QPair<QString, QByteArray>> items) = 0;
};
}  // namespace srilakshmikanthanp::clipbirdesk::network::syncing
