// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ENGINE_H
#define NEBULA_ENGINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace nebula {

class engine {
private:
  GLFWwindow *_window;

public:
  engine();
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
