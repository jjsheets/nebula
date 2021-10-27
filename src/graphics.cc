// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "graphics.h"

// Exception includes
#include "exceptions.h"
#include <string>
#include <set>
#include <limits>
#include <algorithm>

// Logging system includes
#include "loguru.hpp"

namespace nebula {

graphics::graphics(uint32_t width,
    uint32_t height,
    GLFWkeyfun keyCallback,
    bool useValidationLayers)
    : _width(width), _height(height), _window(nullptr), _instance(nullptr),
      _useValidationLayers(useValidationLayers),
      _physicalDevice(VK_NULL_HANDLE), _logicalDevice(VK_NULL_HANDLE),
      _surface(VK_NULL_HANDLE), _swapChain(VK_NULL_HANDLE)
{
  LOG_SCOPE_FUNCTION(INFO);
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
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
}

graphics::~graphics()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_swapChain) {
    vkDestroySwapchainKHR(_logicalDevice, _swapChain, nullptr);
  }
  if (_logicalDevice) {
    vkDestroyDevice(_logicalDevice, nullptr);
  }
  if (_useValidationLayers) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        _instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
      func(_instance, _debugMessenger, nullptr);
    }
  }
  if (_surface) {
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
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
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (_useValidationLayers) {
    createInfo.enabledLayerCount
        = static_cast<uint32_t>(_validationLayers.size());
    createInfo.ppEnabledLayerNames = _validationLayers.data();
    setDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
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
  LOG_SCOPE_FUNCTION(1);
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
  LOG_SCOPE_FUNCTION(1);
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

void graphics::setDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
  LOG_SCOPE_FUNCTION(1);
  createInfo       = {};
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
  createInfo.pNext           = nullptr;
}

void graphics::setValidationCallback()
{
  LOG_SCOPE_FUNCTION(1);
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  setDebugMessengerCreateInfo(createInfo);
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  auto func        = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
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

void graphics::pickPhysicalDevice()
{
  LOG_SCOPE_FUNCTION(INFO);
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    LOG_S(ERROR) << "Vulkan: failed to find GPUs with Vulkan support";
    throw std::runtime_error("Vulkan: failed to find GPUs with Vulkan support");
  }
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
  for (const auto &device : devices) {
    if (deviceIsSuitable(device)) {
      _physicalDevice = device;
      break;
    }
  }
  if (_physicalDevice == VK_NULL_HANDLE) {
    LOG_S(ERROR) << "Vulkan: failed to find a suitable GPU";
    throw std::runtime_error("Vulkan: failed to find a suitable GPU");
  }
}

bool graphics::deviceIsSuitable(VkPhysicalDevice device)
{
  LOG_SCOPE_FUNCTION(1);
  queueFamilyIndices queueFamilies(device, _surface);
  swapChainSupportDetails swapChainDetails(device, _surface);
  return queueFamilies.isComplete() && checkDeviceExtensionSupport(device)
      && swapChainDetails.isSuitable();
}

bool graphics::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
  LOG_SCOPE_FUNCTION(2);
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    return false;
  }
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(
      device, nullptr, &extensionCount, availableExtensions.data());
  std::set<std::string> requiredExtensions(
      _deviceExtensions.begin(), _deviceExtensions.end());
  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }
  return requiredExtensions.empty();
}

graphics::queueFamilyIndices::queueFamilyIndices(
    VkPhysicalDevice device, VkSurfaceKHR surface)
{
  LOG_SCOPE_FUNCTION(1);
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data());
  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      _graphicsFamily = i;
    }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport) {
      _presentFamily = i;
    }
    if (isComplete()) {
      break;
    }
    i++;
  }
}

bool graphics::queueFamilyIndices::isComplete()
{
  LOG_SCOPE_FUNCTION(2);
  return _graphicsFamily.has_value() && _presentFamily.has_value();
}

void graphics::createLogicalDevice()
{
  LOG_SCOPE_FUNCTION(INFO);
  queueFamilyIndices queueFamilies(_physicalDevice, _surface);
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies
      = {queueFamilies._graphicsFamily.value(),
          queueFamilies._presentFamily.value()};
  float queuePriority = 1.0f;
  for (auto queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }
  VkPhysicalDeviceFeatures deviceFeatures {};
  VkDeviceCreateInfo createInfo {};
  createInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.queueCreateInfoCount
      = static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount
      = static_cast<uint32_t>(_deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = _deviceExtensions.data();
  if (_useValidationLayers) {
    createInfo.enabledLayerCount
        = static_cast<uint32_t>(_validationLayers.size());
    createInfo.ppEnabledLayerNames = _validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }
  if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_logicalDevice)
      != VK_SUCCESS)
  {
    LOG_S(ERROR) << "Vulkan: failed to create logical device";
    throw std::runtime_error("Vulkan: failed to create logical device");
  }
  LOG_S(INFO) << "Vulkan: logical device created";
  vkGetDeviceQueue(_logicalDevice,
      queueFamilies._graphicsFamily.value(),
      0,
      &_graphicsQueue);
  vkGetDeviceQueue(
      _logicalDevice, queueFamilies._presentFamily.value(), 0, &_presentQueue);
}

void graphics::createSurface()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface)
      != VK_SUCCESS)
  {
    LOG_S(ERROR) << "Vulkan: failed to create window surface";
    throw std::runtime_error("Vulkan: failed to create window surface");
  }
}

graphics::swapChainSupportDetails::swapChainSupportDetails(
    VkPhysicalDevice device, VkSurfaceKHR surface)
{
  LOG_SCOPE_FUNCTION(1);
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &_capabilities);
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    _formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        device, surface, &formatCount, _formats.data());
  }
  uint32_t modeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      device, surface, &modeCount, nullptr);
  if (modeCount != 0) {
    _presentModes.resize(modeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &modeCount, _presentModes.data());
  }
}

bool graphics::swapChainSupportDetails::isSuitable()
{
  LOG_SCOPE_FUNCTION(2);
  return !_formats.empty() && !_presentModes.empty();
}

VkSurfaceFormatKHR graphics::chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
  LOG_SCOPE_FUNCTION(1);
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
        && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    {
      LOG_S(INFO) << "Format: BGRA, 32-bit";
      LOG_S(INFO) << "Color Space: SRGB, Non-linear";
      return availableFormat;
    }
  }
  LOG_S(INFO) << "Format & Color Space automatically chosen ("
              << availableFormats[0].format << ", "
              << availableFormats[0].colorSpace << ")";
  return availableFormats[0];
}

VkPresentModeKHR graphics::chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes)
{
  LOG_SCOPE_FUNCTION(1);
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      LOG_S(INFO) << "Present Mode: Mailbox";
      return availablePresentMode;
    }
  }
  LOG_S(INFO) << "Present Mode: FIFO";
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D graphics::chooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities)
{
  LOG_SCOPE_FUNCTION(1);
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    LOG_S(INFO) << "Swap Chain Extent: " << capabilities.currentExtent.width
                << "x" << capabilities.currentExtent.height;
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    VkExtent2D actualExtent
        = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width  = std::clamp(actualExtent.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height);
    LOG_S(INFO) << "Swap Chain Extent: " << actualExtent.width << "x"
                << actualExtent.height;

    return actualExtent;
  }
}

void graphics::createSwapChain()
{
  LOG_SCOPE_FUNCTION(INFO);
  swapChainSupportDetails swapChainSupport(_physicalDevice, _surface);
  VkSurfaceFormatKHR surfaceFormat
      = chooseSwapSurfaceFormat(swapChainSupport._formats);
  VkPresentModeKHR presentMode
      = chooseSwapPresentMode(swapChainSupport._presentModes);
  VkExtent2D extent   = chooseSwapExtent(swapChainSupport._capabilities);
  uint32_t imageCount = swapChainSupport._capabilities.minImageCount + 1;
  if (swapChainSupport._capabilities.maxImageCount > 0
      && imageCount > swapChainSupport._capabilities.maxImageCount)
  {
    imageCount = swapChainSupport._capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR createInfo {};
  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = _surface;
  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = surfaceFormat.format;
  createInfo.imageColorSpace  = surfaceFormat.colorSpace;
  createInfo.imageExtent      = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  queueFamilyIndices indices(_physicalDevice, _surface);
  uint32_t queueFamilyIndices[]
      = {indices._graphicsFamily.value(), indices._presentFamily.value()};

  if (indices._graphicsFamily != indices._presentFamily) {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices   = nullptr;
  }
  createInfo.preTransform   = swapChainSupport._capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = presentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = VK_NULL_HANDLE;
  if (vkCreateSwapchainKHR(_logicalDevice, &createInfo, nullptr, &_swapChain)
      != VK_SUCCESS)
  {
    LOG_S(ERROR) << "Vulkan: failed to create swap chain";
    throw std::runtime_error("Vulkan: failed to create swap chain");
  }
  vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &imageCount, nullptr);
  _swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(
      _logicalDevice, _swapChain, &imageCount, _swapChainImages.data());
  _swapChainImageFormat = surfaceFormat.format;
  _swapChainExtent      = extent;
}

} // namespace nebula
