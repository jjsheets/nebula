// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_BIND_H
#define NEBULA_BIND_H

#include <functional>
#include <optional>

namespace nebula {

class bind {
private:
  std::function<void()> _pressHandler;       // argument is repetition count
  std::function<void()> _releaseHandler;     // argument is repetition count
  std::function<void(double)> _deltaHandler; // argument is delta on an axis
  std::optional<int> _key;   // optional, because this may not be bound to a key
  std::optional<int> _mBtn;  // optional, similar reasons
  std::optional<int> _mAxis; // 0 = x, 1 = y, 2 = scroll x, 3 = scroll y
  std::optional<int> _jid;   // when bound to a joystick, this is the GLFW jid
  std::optional<int> _jBtn;  // joystick button to bind to
  std::optional<int> _jAxis; // joystick axis to bind to
public:
  bind(std::function<void()> pressHandler,
      std::function<void()> releaseHandler,
      std::function<void(double)> deltaHandler)
      : _pressHandler(pressHandler), _releaseHandler(releaseHandler),
        _deltaHandler(deltaHandler)
  {
  }
  void bindKey(int key);
  void bindMButton(int mBtn);
  void bindMAxis(int mAxis);
  void bindJButton(int jid, int jBtn);
  void bindJAxis(int jid, int jAxis);
  void unbind();
  void press();
  void release();
  void delta(double delta);
};

} // namespace nebula

#endif // NEBULA_BIND_H
