// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ENGINE_H
#define NEBULA_ENGINE_H

#include "graphics.h"

struct lua_State;

namespace nebula {

class engine {
private:
  static engine *_engine;
  lua_State *_luaState;
  graphics *_graphics;

  static void _keyCallback(
      GLFWwindow *window, int key, int scancode, int action, int mods);
  void keyboardEvent(
      GLFWwindow *window, int key, int scancode, int action, int mods);

public:
  engine();
  ~engine();
  void exit();
  void loop();
#ifndef DOCTEST_CONFIG_DISABLE
  GLFWwindow *getWindowPointer()
  {
    return *_graphics;
  }
#endif
};

} // namespace nebula

#endif // NEBULA_ENGINE_H
