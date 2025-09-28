#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// project headers
#include "controller/controller.hpp"

namespace srilakshmikanthanp::clipbirdesk::controller {
class ClipboardController : public Controller {
 private:  // just for Qt

  Q_OBJECT

 private:

  Q_DISABLE_COPY_MOVE(ClipboardController)

 private:

  clipboard::ApplicationClipboard m_clipboard;

 public:  // Constructors and Destructors

  ClipboardController(QObject *parent = nullptr);
  virtual ~ClipboardController() override;

 public:  // Public Member Function

  clipboard::ApplicationClipboard &getClipboard();
};
}
