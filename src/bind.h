// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_BIND_H
#define NEBULA_BIND_H

#include <functional>
#include <optional>
#include <map>
#include <tuple>
#include <string>
#include <memory>

namespace nebula {

class bind : std::enable_shared_from_this<bind> {
public:
  enum struct action
  {
    press,
    release
  };
  union modifier {
    struct {
      bool shift  : 1;
      bool ctrl   : 1;
      bool alt    : 1;
      bool super  : 1;
      bool left   : 1;
      bool middle : 1;
      bool right  : 1;
    };
    int bits;
  };

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
  modifier _modifiers;
  std::string _category;
  std::string _name;
  std::string _bind;
  bool _bound;

  static std::map<std::tuple<int, int>, std::shared_ptr<bind>> _keyBinds;
  static std::map<std::tuple<int, int>, std::shared_ptr<bind>> _mBtnBinds;
  static std::map<std::tuple<int, int>, std::shared_ptr<bind>> _mAxisBinds;
  static std::map<std::tuple<int, int, int>, std::shared_ptr<bind>> _jBtnBinds;
  static std::map<std::tuple<int, int, int>, std::shared_ptr<bind>> _jAxisBinds;

  bind()
      : _pressHandler(nullptr), _releaseHandler(nullptr),
        _deltaHandler(nullptr), _category(""), _name(""), _bind(""),
        _bound(false)
  {
  }
  bind(std::function<void()> pressHandler,
      std::function<void()> releaseHandler,
      std::function<void(double)> deltaHandler,
      const std::string &category,
      const std::string &name)
      : _pressHandler(pressHandler), _releaseHandler(releaseHandler),
        _deltaHandler(deltaHandler), _category(category), _name(name),
        _bind(""), _bound(false)
  {
  }
  void press();
  void release();
  void delta(double delta);

public:
  void bindKey(int key, modifier mods);
  void bindMButton(int mBtn, modifier mods);
  void bindMAxis(int mAxis, modifier mods);
  void bindJButton(int jid, int jBtn, modifier mods);
  void bindJAxis(int jid, int jAxis, modifier mods);
  void unbind();
  bool bound()
  {
    return _bound;
  }

  [[nodiscard]] static std::shared_ptr<bind> create(
      std::function<void()> pressHandler,
      std::function<void()> releaseHandler,
      std::function<void(double)> deltaHandler,
      const std::string &category,
      const std::string &name)
  {
    return std::shared_ptr<bind>(
        new bind(pressHandler, releaseHandler, deltaHandler, category, name));
  }
  static void keyboardEvent(int key, action event, modifier mods);
  static void mouseButtonEvent(int mBtn, action event, modifier mods);
  static void mouseAxisEvent(int mAxis, double delta, modifier mods);
  static void joystickButtonEvent(
      int jid, int jBtn, action event, modifier mods);
  static void joystickAxisEvent(
      int jid, int jAxis, double delta, modifier mods);
};

} // namespace nebula

#endif // NEBULA_BIND_H
