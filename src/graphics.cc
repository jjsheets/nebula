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
    : _width(width), _height(height), _window(nullptr), _instance(nullptr),
      _useValidationLayers(useValidationLayers)
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
  createVulkanInstance();
  if (_useValidationLayers) {
    setValidationCallback();
  }
}

graphics::~graphics()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_useValidationLayers) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        _instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(_instance, _debugMessenger, nullptr);
    }
  }
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

void graphics::createVulkanInstance()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_useValidationLayers && !checkValidationLayerSupport()) {
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
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = &appInfo;
  auto extensions                    = getRequiredExtensions();
  createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();
  if (_useValidationLayers) {
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

std::vector<const char *> graphics::getRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions
      = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> extensions(
      glfwExtensions, glfwExtensions + glfwExtensionCount);
  if (_useValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL graphics::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
  std::string typeString;
  if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
    typeString = "(gen)";
  } else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
    typeString = "(val)";
  } else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
    typeString = "(perf)";
  }
  if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    LOG_S(1) << "Vulkan" << typeString << ": " << pCallbackData->pMessage;
  } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    LOG_S(INFO) << "Vulkan" << typeString << ": " << pCallbackData->pMessage;
  } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    LOG_S(WARNING) << "Vulkan" << typeString << ": " << pCallbackData->pMessage;
  } else if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    LOG_S(ERROR) << "Vulkan" << typeString << ": " << pCallbackData->pMessage;
  }

  return VK_FALSE;
}

void graphics::setValidationCallback()
{
  VkDebugUtilsMessengerCreateInfoEXT createInfo {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                         | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = graphics::debugCallback;
  createInfo.pUserData       = nullptr;
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      _instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    if (func(_instance, &createInfo, nullptr, &_debugMessenger) == VK_SUCCESS) {
      return;
    }
  }
  LOG_S(ERROR) << "Vulkan: failed to set up validation layer callback";
  throw std::runtime_error(
      "Vulkan: failed to set up validation layer callback");
}

} // namespace nebula
