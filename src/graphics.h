// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_GRAPHICS_H
#define NEBULA_GRAPHICS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace nebula {

class graphics {
private:
  uint32_t _width;
  uint32_t _height;
  GLFWwindow *_window;
  VkInstance _instance;

  void createVulkanInstance();

public:
  graphics(uint32_t width, uint32_t height, GLFWkeyfun keyCallback);
  ~graphics();
  operator GLFWwindow *()
  {
    return _window;
  }
};

} // namespace nebula

#endif // NEBULA_GRAPHICS_H
