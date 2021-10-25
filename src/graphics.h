// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_GRAPHICS_H
#define NEBULA_GRAPHICS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace nebula {

class graphics {
private:
  uint32_t _width;
  uint32_t _height;
  GLFWwindow *_window;
  VkInstance _instance;

  const std::vector<const char *> _validationLayers
      = {"VK_LAYER_KHRONOS_validation"};

  void createVulkanInstance(bool useValidationLayers);
  bool checkValidationLayerSupport();

public:
  graphics(uint32_t width,
      uint32_t height,
      GLFWkeyfun keyCallback,
      bool useValidationLayers);
  ~graphics();
  operator GLFWwindow *()
  {
    return _window;
  }
};

} // namespace nebula

#endif // NEBULA_GRAPHICS_H
