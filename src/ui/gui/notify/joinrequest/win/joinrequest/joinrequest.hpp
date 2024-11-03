#ifdef _WIN32 // Only for windows using wintoast see https://github.com/mohabouje/WinToast
#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

#include <wintoastlib.h>

#include <QObject>
#include <QMetaObject>

#include "constants/constants.hpp"
#include "types/device.hpp"

namespace srilakshmikanthanp::clipbirdesk::ui::gui::notify {
/**
 * @brief Notification class
 */
class JoinRequest : public QObject, public WinToastLib::IWinToastHandler {
 private:  // disable copy and move for this class

  Q_DISABLE_COPY_MOVE(JoinRequest)

 private:  // just for Qt

  Q_OBJECT

 signals:  // signals
  // called when user Accepted
  void onAccept() const;

 signals:  // signals
  // called when user Rejected
  void onReject() const;

 private:  // Member Functions

  /**
   * @brief Called when user Dismissed
   */
  void toastDismissed(WinToastDismissalReason state) const;

  /**
   * @brief Called when user Activated
   */
  void toastActivated() const;

  /**
   * @brief Called when user Failed
   */
  void toastFailed() const;

  /**
   * @brief Called when user Activated
   */
  void toastActivated(int actionIndex) const;

  /**
   * @brief Accept Impl
   */
  void acceptImpl() const;

  /**
   * @brief Reject Impl
   */
  void rejectImpl() const;

 public:

  /**
   * @brief Construct a new Abstract object
   *
   * @param parent
   */
  explicit JoinRequest(QObject *parent = nullptr);

  /**
   * @brief Show the notification
   */
  void show(const types::Device &device);

  /**
   * @brief Destroy the Status object
   *
   */
  virtual ~JoinRequest() = default;
};
}  // namespace srilakshmikanthanp::clipbirdesk::ui::gui::notify
#endif  // _WIN32
