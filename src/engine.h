// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ENGINE_H
#define NEBULA_ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace nebula {

class engine {
private:
  static engine *_engine;
  GLFWwindow *_window;

  static void _keyCallback(
      GLFWwindow *window, int key, int scancode, int action, int mods);
  void keyboardEvent(
      GLFWwindow *window, int key, int scancode, int action, int mods);

public:
  engine(int argc, char *argv[]);
  ~engine();
  GLFWwindow *getWindowPointer()
  {
    return _window;
  }
  void exit();
  void loop();
};

} // namespace nebula

#endif // NEBULA_ENGINE_H
