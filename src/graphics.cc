// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "graphics.h"

// Exception includes
#include "exceptions.h"
#include <string>

// Logging system includes
#include "loguru.hpp"

namespace nebula {

graphics::graphics(uint32_t width,
    uint32_t height,
    GLFWkeyfun keyCallback,
    bool useValidationLayers)
    : _width(width), _height(height), _window(nullptr), _instance(nullptr)
{
  LOG_S(INFO) << "GLFW " << glfwGetVersionString();
  if (!glfwInit()) {
    LOG_S(ERROR) << "Could not initialize GLFW: Platform does not meet minimum "
                    "requirements for GLFW initialization";
    throw glfwException();
  }
  LOG_S(INFO) << "GLFW library loaded";
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  _window = glfwCreateWindow(_width, _height, "Nebula", nullptr, nullptr);
  if (!_window) {
    LOG_S(ERROR) << "GLFW: Window creation failed";
    glfwTerminate();
    throw glfwException();
  }
  LOG_S(INFO) << "GLFW: Game window created (" << _width << "x" << _height
              << ")";
  glfwSetKeyCallback(_window, keyCallback);
  createVulkanInstance(useValidationLayers);
}

graphics::~graphics()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_instance) {
    vkDestroyInstance(_instance, nullptr);
    LOG_S(INFO) << "Vulkan: instance destroyed";
  }
  if (_window) {
    glfwDestroyWindow(_window);
    LOG_S(INFO) << "GLFW: Game window destroyed";
  }
  glfwTerminate();
  LOG_S(INFO) << "GLFW: Terminated";
}

void graphics::createVulkanInstance(bool useValidationLayers)
{
  LOG_SCOPE_FUNCTION(INFO);
  if (useValidationLayers && !checkValidationLayerSupport()) {
    LOG_S(ERROR) << "Vulkan: validation layers requested, but not available";
    throw std::runtime_error(
        "Vulkan: validation layers requested, but not available");
  }
  VkApplicationInfo appInfo {};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "Nebula";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "Nebula";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_0;
  VkInstanceCreateInfo createInfo {};
  createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions
      = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  createInfo.enabledExtensionCount   = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  if (useValidationLayers) {
    createInfo.enabledLayerCount
        = static_cast<uint32_t>(_validationLayers.size());
    createInfo.ppEnabledLayerNames = _validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }
  if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
    LOG_S(ERROR) << "Vulkan: failed to create instance";
    throw std::runtime_error("Vulkan: failed to create instance");
  }
  LOG_S(INFO) << "Vulkan: instance created";
}

bool graphics::checkValidationLayerSupport()
{
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
  for (const std::string layerName : _validationLayers) {
    bool layerFound = false;
    for (const auto &layerProperties : availableLayers) {
      if (layerName == layerProperties.layerName) {
        layerFound = true;
        break;
      }
    }
    if (!layerFound) {
      return false;
    }
  }
  return true;
}

} // namespace nebula
