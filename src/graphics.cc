// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "graphics.h"

// Exception includes
#include "exceptions.h"
#include <set>
#include <limits>
#include <algorithm>
#include <ios>
#include <iomanip>
#include <fstream>
#include <cstring>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

// Logging system includes
#include "loguru.hpp"

#ifndef DOCTEST_CONFIG_DISABLE
  #include <doctest.h>
  #include <doctest/trompeloeil.hpp>
  #include "../test/vulkan-mock.h"

SCENARIO("class graphics")
{
  GIVEN("a graphics object")
  {
    // Set up the expectations for this test in a mock object
    vulkanMock vkMock;
    vkMock.mockGraphics();

    nebula::graphics gfx(
        1600, 900, [](GLFWwindow *, int, int, int, int) {}, true);
    THEN("it should not throw when a frame is drawn")
    {
      REQUIRE_NOTHROW(gfx.drawFrame());
    }
  }
  GIVEN("a graphics object without validation layers, testing alternate code "
        "paths")
  {
    // Set up the expectations for this test in a mock object
    vulkanMock vkMock;
    vkMock.enableSeparateQueues();
    vkMock.enableAltSurfaceFormat();
    vkMock.enableMailobxPresentMode();
    vkMock.enableAltSurfaceCaps();
    vkMock.setPhysDeviceType(VK_PHYSICAL_DEVICE_TYPE_CPU);
    vkMock.mockGraphics();

    nebula::graphics gfx(
        1600, 900, [](GLFWwindow *, int, int, int, int) {}, false);
    vkMock.framebufferResize(1280, 720);
    THEN("it should not throw when a frame is drawn")
    {
      REQUIRE_NOTHROW(gfx.drawFrame());
      vkMock.setSwapChainOutOfDate();
      REQUIRE_NOTHROW(gfx.drawFrame());
    }
  }
}

#endif

namespace nebula {

const std::vector<vertex> vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

graphics::graphics(uint32_t width,
    uint32_t height,
    GLFWkeyfun keyCallback,
    bool useValidationLayers)
    : _width(width), _height(height), _window(nullptr), _instance(nullptr),
      _useValidationLayers(useValidationLayers),
      _physicalDevice(VK_NULL_HANDLE), _logicalDevice(VK_NULL_HANDLE),
      _surface(VK_NULL_HANDLE), _swapChain(VK_NULL_HANDLE),
      _renderPass(VK_NULL_HANDLE), _commandPool(VK_NULL_HANDLE),
      _currentFrame(0), _framebufferResized(false),
      _vertexBuffer(VK_NULL_HANDLE), _vertexBufferMemory(VK_NULL_HANDLE)
{
  LOG_SCOPE_FUNCTION(INFO);
  initGLFW(keyCallback);
  createVulkanInstance();
  if (_useValidationLayers) {
    setValidationCallback();
  }
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  _pipeline = new pipeline(_logicalDevice,
      "shaders/shader.vert.spv",
      "shaders/shader.frag.spv",
      _swapChainExtent,
      _renderPass);
  createFramebuffers();
  createCommandPool();
  createVertexBuffer();
  createIndexBuffer();
  createUniformBuffers();
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
  createSyncObjects();
}

graphics::~graphics()
{
  LOG_SCOPE_FUNCTION(INFO);
  destroySynchronization();
  cleanupSwapChain();
  destroyBuffers();
  destroyLogicalDevice();
  destroyVulkanInstance();
  destroyGLFW();
}

void graphics::destroySynchronization()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_logicalDevice) {
    vkDeviceWaitIdle(_logicalDevice);
  }
  for (size_t i = 0; i < _maxFramesInFlight; i++) {
    vkDestroySemaphore(_logicalDevice, _renderFinished[i], nullptr);
    vkDestroySemaphore(_logicalDevice, _imageAvailable[i], nullptr);
    vkDestroyFence(_logicalDevice, _inFlightFence[i], nullptr);
  }
}

void graphics::destroyLogicalDevice()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_commandPool) {
    vkDestroyCommandPool(_logicalDevice, _commandPool, nullptr);
  }
  if (_logicalDevice) {
    vkDestroyDevice(_logicalDevice, nullptr);
  }
}

void graphics::destroyBuffers()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_indexBuffer) {
    vkDestroyBuffer(_logicalDevice, _indexBuffer, nullptr);
  }
  if (_indexBufferMemory) {
    vkFreeMemory(_logicalDevice, _indexBufferMemory, nullptr);
  }
  if (_vertexBuffer) {
    vkDestroyBuffer(_logicalDevice, _vertexBuffer, nullptr);
  }
  if (_vertexBufferMemory) {
    vkFreeMemory(_logicalDevice, _vertexBufferMemory, nullptr);
  }
}

void graphics::destroyVulkanInstance()
{
  LOG_SCOPE_FUNCTION(INFO);
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
}

void graphics::destroyGLFW()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_window) {
    glfwDestroyWindow(_window);
    LOG_S(INFO) << "GLFW: Game window destroyed";
  }
  glfwTerminate();
  LOG_S(INFO) << "GLFW: Terminated";
}

void graphics::initGLFW(GLFWkeyfun keyCallback)
{
  LOG_SCOPE_FUNCTION(INFO);
  LOG_S(INFO) << "GLFW " << glfwGetVersionString();
  if (!glfwInit()) {
    throw glfwException(
        "Could not initialize GLFW: Platform does not meet minimum "
        "requirements for GLFW initialization");
  }
  LOG_S(INFO) << "GLFW library loaded";
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  _window = glfwCreateWindow(_width, _height, "Nebula", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw glfwException("GLFW: Window creation failed");
  }
  LOG_S(INFO) << "GLFW: Game window created (" << _width << "x" << _height
              << ")";
  glfwSetWindowUserPointer(_window, this);
  glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
  glfwSetKeyCallback(_window, keyCallback);
}

void graphics::setClose(bool close)
{
  glfwSetWindowShouldClose(_window, close);
}

void graphics::framebufferResizeCallback(
    GLFWwindow *window, int width, int height)
{
  auto gfx = reinterpret_cast<graphics *>(glfwGetWindowUserPointer(window));
  gfx->_framebufferResized = true;
}

void graphics::createVulkanInstance()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_useValidationLayers && !checkValidationLayerSupport()) {
    throw nebulaException(
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
    throw nebulaException("Vulkan: failed to create instance");
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
    typeString = " (gen)";
  } else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
    typeString = " (val)";
  } else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
    typeString = " (perf)";
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
  throw nebulaException("Vulkan: failed to set up validation layer callback");
}

void graphics::pickPhysicalDevice()
{
  LOG_SCOPE_FUNCTION(INFO);
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw nebulaException("Vulkan: failed to find GPUs with Vulkan support");
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
    throw nebulaException("Vulkan: failed to find a suitable GPU");
  }
  logPhysicalDevice();
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
  if (_graphicsFamily)
    LOG_S(1) << "graphicsFamily: " << _graphicsFamily.value();
  if (_presentFamily)
    LOG_S(1) << "presentFamily: " << _presentFamily.value();
}

bool graphics::queueFamilyIndices::isComplete()
{
  LOG_SCOPE_FUNCTION(2);
  return _graphicsFamily.has_value() && _presentFamily.has_value();
}

void graphics::populateQueueCreateInfos(queueFamilyIndices &queueFamilies,
    std::vector<VkDeviceQueueCreateInfo> &queueCreateInfos)
{
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
}

void graphics::createLogicalDevice()
{
  LOG_SCOPE_FUNCTION(INFO);
  queueFamilyIndices queueFamilies(_physicalDevice, _surface);
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  populateQueueCreateInfos(queueFamilies, queueCreateInfos);
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
    throw nebulaException("Vulkan: failed to create logical device");
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
    throw nebulaException("Vulkan: failed to create window surface");
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

uint32_t graphics::swapChainSupportDetails::swapChainImageCount()
{
  uint32_t imageCount = _capabilities.minImageCount + 1;
  if (_capabilities.maxImageCount > 0
      && imageCount > _capabilities.maxImageCount) {
    imageCount = _capabilities.maxImageCount;
  }
  return imageCount;
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
  uint32_t imageCount = swapChainSupport.swapChainImageCount();
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
    throw nebulaException("Vulkan: failed to create swap chain");
  }
  vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &imageCount, nullptr);
  _swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(
      _logicalDevice, _swapChain, &imageCount, _swapChainImages.data());
  _swapChainImageFormat = surfaceFormat.format;
  _swapChainExtent      = extent;
}

void graphics::recreateSwapChain()
{
  LOG_SCOPE_FUNCTION(1);
  int width = 0, height = 0;
  glfwGetFramebufferSize(_window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(_window, &width, &height);
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(_logicalDevice);
  cleanupSwapChain();
  createSwapChain();
  createImageViews();
  createRenderPass();
  _pipeline = new pipeline(_logicalDevice,
      "shaders/shader.vert.spv",
      "shaders/shader.frag.spv",
      _swapChainExtent,
      _renderPass);
  createFramebuffers();
  createUniformBuffers();
  createDescriptorPool();
  createDescriptorSets();
  createCommandBuffers();
}

void graphics::cleanupSwapChain()
{
  LOG_SCOPE_FUNCTION(1);
  for (auto framebuffer : _swapChainFramebuffers) {
    vkDestroyFramebuffer(_logicalDevice, framebuffer, nullptr);
  }
  vkFreeCommandBuffers(_logicalDevice,
      _commandPool,
      static_cast<uint32_t>(_commandBuffers.size()),
      _commandBuffers.data());
  delete _pipeline;
  if (_renderPass) {
    vkDestroyRenderPass(_logicalDevice, _renderPass, nullptr);
  }
  for (auto imageView : _swapChainImageViews) {
    vkDestroyImageView(_logicalDevice, imageView, nullptr);
  }
  if (_swapChain) {
    vkDestroySwapchainKHR(_logicalDevice, _swapChain, nullptr);
  }
  for (size_t i = 0; i < _swapChainImages.size(); i++) {
    vkDestroyBuffer(_logicalDevice, _uniformBuffers[i], nullptr);
    vkFreeMemory(_logicalDevice, _uniformBuffersMemory[i], nullptr);
  }
  vkDestroyDescriptorPool(_logicalDevice, _descriptorPool, nullptr);
}

void graphics::createImageViews()
{
  LOG_SCOPE_FUNCTION(INFO);
  _swapChainImageViews.resize(_swapChainImages.size());
  for (size_t i = 0; i < _swapChainImages.size(); i++) {
    VkImageViewCreateInfo createInfo {};
    createInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image        = _swapChainImages[i];
    createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format       = _swapChainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;
    if (vkCreateImageView(
            _logicalDevice, &createInfo, nullptr, &_swapChainImageViews[i])
        != VK_SUCCESS)
    {
      throw nebulaException("Vulkan: failed to create image views");
    }
  }
}

void graphics::logMemoryType(VkMemoryType &mType)
{
  LOG_SCOPE_FUNCTION(9);
  std::string flags = "";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    flags += ", device local";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    flags += ", host visible";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
    flags += ", host coherent";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)
    flags += ", host cached";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
    flags += ", lazy alloc";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT)
    flags += ", protected";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD)
    flags += ", device coherent";
  if (mType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD)
    flags += ", device uncached";
  LOG_S(INFO) << "  Heap " << mType.heapIndex << ": flags " << std::hex
              << std::setfill('0') << std::setw(4) << std::uppercase
              << mType.propertyFlags << flags;
}

void graphics::logMemoryHeap(VkMemoryHeap &mHeap, uint32_t i)
{
  LOG_SCOPE_FUNCTION(9);
  std::string flags = "";
  if (mHeap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
    flags += ", device local";
  if (mHeap.flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT)
    flags += ", multi-instance";
  double size = (double)(mHeap.size) / 1048576.0;
  LOG_S(INFO) << "  Heap " << i << ": " << size << "MB" << flags;
}

void graphics::logPhysicalDevice()
{
  LOG_SCOPE_FUNCTION(9);
  VkPhysicalDeviceProperties props;
  vkGetPhysicalDeviceProperties(_physicalDevice, &props);
  LOG_S(INFO) << "Device: " << props.deviceName;
  std::string dType = "Other";
  if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
    dType = "Integrated";
  else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    dType = "Discrete";
  else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
    dType = "Virtual";
  else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
    dType = "CPU";
  LOG_S(INFO) << "Device Type: " << dType;
  LOG_S(INFO) << "API v" << VK_VERSION_MAJOR(props.apiVersion) << "."
              << VK_VERSION_MINOR(props.apiVersion) << "."
              << VK_VERSION_PATCH(props.apiVersion);
  LOG_S(INFO) << "Driver v" << VK_VERSION_MAJOR(props.driverVersion) << "."
              << VK_VERSION_MINOR(props.driverVersion) << "."
              << VK_VERSION_PATCH(props.driverVersion);
  LOG_S(INFO) << "vID:dID " << std::hex << std::setw(4) << std::setfill('0')
              << std::uppercase << (props.vendorID & 0xFFFF) << ":" << std::hex
              << std::setw(4) << std::setfill('0') << std::uppercase
              << (props.deviceID & 0xFFFF);
  VkPhysicalDeviceMemoryProperties mem;
  vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &mem);
  LOG_S(INFO) << "Heap Sizes";
  for (uint32_t i = 0; i < mem.memoryHeapCount; i++) {
    logMemoryHeap(mem.memoryHeaps[i], i);
  }
  LOG_S(INFO) << "Heap Flags";
  for (uint32_t i = 0; i < mem.memoryTypeCount; i++) {
    logMemoryType(mem.memoryTypes[i]);
  }
}

graphics::pipeline::pipeline(VkDevice device,
    const std::string &vertShader,
    const std::string &fragShader,
    VkExtent2D &swapChainExtent,
    VkRenderPass renderPass)
    : _device(device), _pipelineLayout(VK_NULL_HANDLE),
      _graphicsPipeline(VK_NULL_HANDLE), _descriptorSetLayout(VK_NULL_HANDLE)
{
  LOG_SCOPE_FUNCTION(INFO);
  auto vertShaderCode             = readFile(vertShader);
  auto fragShaderCode             = readFile(fragShader);
  VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
  VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

  createDescriptorSetLayout();
  setupVertShader(vertShaderModule, "main");
  setupFragShader(fragShaderModule, "main");
  VkPipelineShaderStageCreateInfo shaderStages[]
      = {_vertShaderStageInfo, _fragShaderStageInfo};
  setupVertexInputState();
  setupInputAssemblyState();
  setupViewport(swapChainExtent);
  setupViewportState();
  setupRasterState();
  setupMultisampling();
  setupColorBlend();
  setupPipelineLayout(renderPass);
  _pipelineInfo.stageCount = 2;
  _pipelineInfo.pStages    = shaderStages;
  if (vkCreateGraphicsPipelines(_device,
          VK_NULL_HANDLE,
          1,
          &_pipelineInfo,
          nullptr,
          &_graphicsPipeline)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create graphics pipeline");
  }
  vkDestroyShaderModule(_device, fragShaderModule, nullptr);
  vkDestroyShaderModule(_device, vertShaderModule, nullptr);
}

void graphics::pipeline::setupVertShader(
    VkShaderModule &vertShaderModule, const char *entryPoint)
{
  _vertShaderStageInfo = {};
  _vertShaderStageInfo.sType
      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  _vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  _vertShaderStageInfo.module = vertShaderModule;
  _vertShaderStageInfo.pName  = entryPoint;
}

void graphics::pipeline::setupFragShader(
    VkShaderModule &fragShaderModule, const char *entryPoint)
{
  _fragShaderStageInfo = {};
  _fragShaderStageInfo.sType
      = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  _fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  _fragShaderStageInfo.module = fragShaderModule;
  _fragShaderStageInfo.pName  = entryPoint;
}

void graphics::pipeline::setupVertexInputState()
{
  _vertexInputInfo = {};
  _vertexInputInfo.sType
      = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex::getBindingDesc();
  vertex::getAttributeDesc();
  _vertexInputInfo.vertexBindingDescriptionCount = 1;
  _vertexInputInfo.vertexAttributeDescriptionCount
      = static_cast<uint32_t>(vertex::_attribDesc.size());
  _vertexInputInfo.pVertexBindingDescriptions   = &vertex::_bindDesc;
  _vertexInputInfo.pVertexAttributeDescriptions = vertex::_attribDesc.data();
}

void graphics::pipeline::setupInputAssemblyState()
{
  _inputAssembly = {};
  _inputAssembly.sType
      = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  _inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  _inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void graphics::pipeline::setupViewport(VkExtent2D &swapChainExtent)
{
  _viewport          = {};
  _viewport.x        = 0.0f;
  _viewport.y        = 0.0f;
  _viewport.width    = (float)swapChainExtent.width;
  _viewport.height   = (float)swapChainExtent.height;
  _viewport.minDepth = 0.0f;
  _viewport.maxDepth = 1.0f;
  _scissor           = {};
  _scissor.offset    = {0, 0};
  _scissor.extent    = swapChainExtent;
}

void graphics::pipeline::setupViewportState()
{
  _viewportState       = {};
  _viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  _viewportState.viewportCount = 1;
  _viewportState.pViewports    = &_viewport;
  _viewportState.scissorCount  = 1;
  _viewportState.pScissors     = &_scissor;
}

void graphics::pipeline::setupRasterState()
{
  _rasterizer = {};
  _rasterizer.sType
      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  _rasterizer.depthClampEnable        = VK_FALSE;
  _rasterizer.rasterizerDiscardEnable = VK_FALSE;
  _rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
  _rasterizer.lineWidth               = 1.0f;
  _rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
  _rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  _rasterizer.depthBiasEnable         = VK_FALSE;
}

void graphics::pipeline::setupMultisampling()
{
  _multisampling = {};
  _multisampling.sType
      = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  _multisampling.sampleShadingEnable  = VK_FALSE;
  _multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
}

void graphics::pipeline::setupColorBlend()
{
  _colorBlendAttachment = {};
  _colorBlendAttachment.colorWriteMask
      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
      | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  _colorBlendAttachment.blendEnable = VK_FALSE;
  _colorBlending                    = {};
  _colorBlending.sType
      = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  _colorBlending.logicOpEnable   = VK_FALSE;
  _colorBlending.logicOp         = VK_LOGIC_OP_COPY;
  _colorBlending.attachmentCount = 1;
  _colorBlending.pAttachments    = &_colorBlendAttachment;
}

void graphics::pipeline::setupPipelineLayout(VkRenderPass renderPass)
{
  _pipelineLayoutInfo       = {};
  _pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  _pipelineLayoutInfo.setLayoutCount = 1;
  _pipelineLayoutInfo.pSetLayouts    = &_descriptorSetLayout;
  if (vkCreatePipelineLayout(
          _device, &_pipelineLayoutInfo, nullptr, &_pipelineLayout)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create pipeline layout");
  }
  _pipelineInfo       = {};
  _pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  _pipelineInfo.pVertexInputState   = &_vertexInputInfo;
  _pipelineInfo.pInputAssemblyState = &_inputAssembly;
  _pipelineInfo.pViewportState      = &_viewportState;
  _pipelineInfo.pRasterizationState = &_rasterizer;
  _pipelineInfo.pMultisampleState   = &_multisampling;
  _pipelineInfo.pColorBlendState    = &_colorBlending;
  _pipelineInfo.layout              = _pipelineLayout;
  _pipelineInfo.renderPass          = renderPass;
  _pipelineInfo.subpass             = 0;
}

graphics::pipeline::~pipeline()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_descriptorSetLayout) {
    vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
  }
  if (_graphicsPipeline) {
    vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
  }
  if (_pipelineLayout) {
    vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
  }
}

std::vector<char> graphics::pipeline::readFile(const std::string &filename)
{
  LOG_SCOPE_FUNCTION(INFO);
  std::ifstream file(filename, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw nebulaException("failed to open file: " + filename);
  }
  LOG_S(INFO) << "opened shader file: " << filename;
  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return buffer;
}

VkShaderModule graphics::pipeline::createShaderModule(
    const std::vector<char> &code)
{
  LOG_SCOPE_FUNCTION(INFO);
  VkShaderModuleCreateInfo createInfo {};
  createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode    = reinterpret_cast<const uint32_t *>(code.data());
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create shader module");
  }
  return shaderModule;
}

void graphics::createRenderPass()
{
  LOG_SCOPE_FUNCTION(INFO);
  VkAttachmentDescription colorAttachment {};
  colorAttachment.format        = _swapChainImageFormat;
  colorAttachment.samples       = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp       = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  VkAttachmentReference colorAttachmentRef {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  VkSubpassDescription subpass {};
  subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments    = &colorAttachmentRef;
  VkSubpassDependency dependency {};
  dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass    = 0;
  dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  VkRenderPassCreateInfo renderPassInfo {};
  renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments    = &colorAttachment;
  renderPassInfo.subpassCount    = 1;
  renderPassInfo.pSubpasses      = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies   = &dependency;
  if (vkCreateRenderPass(_logicalDevice, &renderPassInfo, nullptr, &_renderPass)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create render pass");
  }
}

void graphics::createFramebuffers()
{
  LOG_SCOPE_FUNCTION(INFO);
  _swapChainFramebuffers.resize(_swapChainImageViews.size());
  for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {_swapChainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo {};
    framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = _renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments    = attachments;
    framebufferInfo.width           = _swapChainExtent.width;
    framebufferInfo.height          = _swapChainExtent.height;
    framebufferInfo.layers          = 1;
    if (vkCreateFramebuffer(_logicalDevice,
            &framebufferInfo,
            nullptr,
            &_swapChainFramebuffers[i])
        != VK_SUCCESS)
    {
      throw nebulaException("Vulkan: failed to create framebuffer");
    }
  }
}

void graphics::createCommandPool()
{
  LOG_SCOPE_FUNCTION(INFO);
  queueFamilyIndices queueFamilies(_physicalDevice, _surface);
  VkCommandPoolCreateInfo poolInfo {};
  poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = queueFamilies._graphicsFamily.value();
  if (vkCreateCommandPool(_logicalDevice, &poolInfo, nullptr, &_commandPool)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create command pool");
  }
}

void graphics::createCommandBuffers()
{
  LOG_SCOPE_FUNCTION(INFO);
  _commandBuffers.resize(_swapChainFramebuffers.size());
  VkCommandBufferAllocateInfo allocInfo {};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool        = _commandPool;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();
  if (vkAllocateCommandBuffers(
          _logicalDevice, &allocInfo, _commandBuffers.data())
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to allocate command buffers");
  }
  for (size_t i = 0; i < _commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw nebulaException("Vulkan: failed to begin recording command buffer");
    }
    VkRenderPassBeginInfo renderPassInfo {};
    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = _renderPass;
    renderPassInfo.framebuffer       = _swapChainFramebuffers[i];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChainExtent;
    VkClearValue clearColor          = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount   = 1;
    renderPassInfo.pClearValues      = &clearColor;
    vkCmdBeginRenderPass(
        _commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(
        _commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipeline);
    VkBuffer vertexBuffers[] = {_vertexBuffer};
    VkDeviceSize offsets[]   = {0};
    vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(
        _commandBuffers[i], _indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(_commandBuffers[i],
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        _pipeline->_pipelineLayout,
        0,
        1,
        &_descriptorSets[i],
        0,
        nullptr);
    vkCmdDrawIndexed(
        _commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    vkCmdEndRenderPass(_commandBuffers[i]);
    if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
      throw nebulaException("Vulkan: failed to record command buffer");
    }
  }
}

void graphics::createSyncObjects()
{
  LOG_SCOPE_FUNCTION(INFO);
  _imageAvailable.resize(_maxFramesInFlight);
  _renderFinished.resize(_maxFramesInFlight);
  _inFlightFence.resize(_maxFramesInFlight);
  _inFlightImage.resize(_swapChainImages.size(), VK_NULL_HANDLE);
  VkSemaphoreCreateInfo semaphoreInfo {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VkFenceCreateInfo fenceInfo {};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  for (size_t i = 0; i < _maxFramesInFlight; i++) {
    if (vkCreateSemaphore(
            _logicalDevice, &semaphoreInfo, nullptr, &_imageAvailable[i])
            != VK_SUCCESS
        || vkCreateSemaphore(
               _logicalDevice, &semaphoreInfo, nullptr, &_renderFinished[i])
               != VK_SUCCESS
        || vkCreateFence(
               _logicalDevice, &fenceInfo, nullptr, &_inFlightFence[i])
               != VK_SUCCESS)
    {
      throw nebulaException("Vulkan: failed to create synchronization objects");
    }
  }
}

void graphics::waitForFrame(size_t f)
{
  vkWaitForFences(_logicalDevice,
      1,
      &_inFlightFence[f],
      VK_TRUE,
      std::numeric_limits<uint64_t>::max());
}

void graphics::waitForImage(size_t f)
{
  vkWaitForFences(_logicalDevice,
      1,
      &_inFlightImage[f],
      VK_TRUE,
      std::numeric_limits<uint64_t>::max());
}

void graphics::submitQueue(uint32_t imageIndex, VkSemaphore signalSemaphores[])
{
  LOG_SCOPE_FUNCTION(9);
  VkSubmitInfo submitInfo {};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {_imageAvailable[_currentFrame]};
  VkPipelineStageFlags waitStages[]
      = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount   = 1;
  submitInfo.pWaitSemaphores      = waitSemaphores;
  submitInfo.pWaitDstStageMask    = waitStages;
  submitInfo.commandBufferCount   = 1;
  submitInfo.pCommandBuffers      = &_commandBuffers[imageIndex];
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores    = signalSemaphores;
  vkResetFences(_logicalDevice, 1, &_inFlightFence[_currentFrame]);
  if (vkQueueSubmit(
          _graphicsQueue, 1, &submitInfo, _inFlightFence[_currentFrame])
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to submit draw command buffer");
  }
}

void graphics::presentQueue(uint32_t imageIndex, VkSemaphore signalSemaphores[])
{
  LOG_SCOPE_FUNCTION(9);
  VkPresentInfoKHR presentInfo {};
  presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores    = signalSemaphores;
  VkSwapchainKHR swapChains[]    = {_swapChain};
  presentInfo.swapchainCount     = 1;
  presentInfo.pSwapchains        = swapChains;
  presentInfo.pImageIndices      = &imageIndex;
  VkResult result = vkQueuePresentKHR(_presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
      || _framebufferResized)
  {
    _framebufferResized = false;
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw nebulaException("Vulkan: failed to present swap chain image");
  }
}

void graphics::drawFrame()
{
  LOG_SCOPE_FUNCTION(9);
  waitForFrame(_currentFrame);
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(_logicalDevice,
      _swapChain,
      std::numeric_limits<uint64_t>::max(),
      _imageAvailable[_currentFrame],
      VK_NULL_HANDLE,
      &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw nebulaException("Vulkan: failed to acquire swap chain image");
  }
  if (_inFlightImage[imageIndex] != VK_NULL_HANDLE) {
    waitForImage(imageIndex);
  }
  updateUniformBuffer(imageIndex);
  _inFlightImage[imageIndex]     = _inFlightFence[_currentFrame];
  VkSemaphore signalSemaphores[] = {_renderFinished[_currentFrame]};
  submitQueue(imageIndex, signalSemaphores);
  presentQueue(imageIndex, signalSemaphores);
  _currentFrame = (_currentFrame + 1) % _maxFramesInFlight;
}

void graphics::updateUniformBuffer(uint32_t currentImage)
{
  LOG_SCOPE_FUNCTION(INFO);
  static auto startTime = std::chrono::high_resolution_clock::now();
  auto currentTime      = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
      currentTime - startTime)
                   .count();
  uniformBufferObject ubo {};
  ubo.model = glm::rotate(
      glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),
      _swapChainExtent.width / (float)_swapChainExtent.height,
      0.1f,
      10.0f);
  ubo.proj[1][1] *= -1;
  void *data;
  vkMapMemory(_logicalDevice,
      _uniformBuffersMemory[currentImage],
      0,
      sizeof(ubo),
      0,
      &data);
  memcpy(data, &ubo, sizeof(ubo));
  vkUnmapMemory(_logicalDevice, _uniformBuffersMemory[currentImage]);
}

VkVertexInputBindingDescription vertex::_bindDesc;
std::array<VkVertexInputAttributeDescription, 2> vertex::_attribDesc;

void vertex::getBindingDesc()
{
  _bindDesc           = {};
  _bindDesc.binding   = 0;
  _bindDesc.stride    = sizeof(vertex);
  _bindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void vertex::getAttributeDesc()
{
  _attribDesc             = {};
  _attribDesc[0].binding  = 0;
  _attribDesc[0].location = 0;
  _attribDesc[0].format   = VK_FORMAT_R32G32_SFLOAT;
  _attribDesc[0].offset   = offsetof(vertex, pos);
  _attribDesc[1].binding  = 0;
  _attribDesc[1].location = 1;
  _attribDesc[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
  _attribDesc[1].offset   = offsetof(vertex, color);
}

void graphics::createBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer &buffer,
    VkDeviceMemory &bufferMemory)
{
  LOG_SCOPE_FUNCTION(INFO);
  VkBufferCreateInfo bufferInfo {};
  bufferInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size        = size;
  bufferInfo.usage       = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vkCreateBuffer(_logicalDevice, &bufferInfo, nullptr, &buffer)
      != VK_SUCCESS) {
    throw NEBULA_EXCEPTIONS_H("Vulkan: failed to create buffer");
  }
  LOG_S(INFO) << "Vulkan: buffer created";
  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(_logicalDevice, buffer, &memRequirements);
  VkMemoryAllocateInfo allocInfo {};
  allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex
      = findMemoryType(memRequirements.memoryTypeBits, properties);
  if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &bufferMemory)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to allocate buffer memory");
  }
  LOG_S(INFO) << "Vulkan: memory allocated";
  vkBindBufferMemory(_logicalDevice, buffer, bufferMemory, 0);
}

void graphics::createVertexBuffer()
{
  LOG_SCOPE_FUNCTION(INFO);
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
          | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);
  void *data;
  vkMapMemory(_logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(_logicalDevice, stagingBufferMemory);
  LOG_S(INFO) << "Vulkan: vertex data transferred";
  createBuffer(bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      _vertexBuffer,
      _vertexBufferMemory);
  copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);
  vkDestroyBuffer(_logicalDevice, stagingBuffer, nullptr);
  vkFreeMemory(_logicalDevice, stagingBufferMemory, nullptr);
}

void graphics::createIndexBuffer()
{
  LOG_SCOPE_FUNCTION(INFO);
  VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(bufferSize,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
          | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      stagingBuffer,
      stagingBufferMemory);
  void *data;
  vkMapMemory(_logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(_logicalDevice, stagingBufferMemory);
  createBuffer(bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      _indexBuffer,
      _indexBufferMemory);
  copyBuffer(stagingBuffer, _indexBuffer, bufferSize);
  vkDestroyBuffer(_logicalDevice, stagingBuffer, nullptr);
  vkFreeMemory(_logicalDevice, stagingBufferMemory, nullptr);
}

void graphics::copyBuffer(
    VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
  LOG_SCOPE_FUNCTION(1);
  VkCommandBufferAllocateInfo allocInfo {};
  allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool        = _commandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(_logicalDevice, &allocInfo, &commandBuffer);
  VkCommandBufferBeginInfo beginInfo {};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer(commandBuffer, &beginInfo);
  VkBufferCopy copyRegion {};
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
  vkEndCommandBuffer(commandBuffer);
  VkSubmitInfo submitInfo {};
  submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers    = &commandBuffer;
  vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(_graphicsQueue);
  vkFreeCommandBuffers(_logicalDevice, _commandPool, 1, &commandBuffer);
}

uint32_t graphics::findMemoryType(
    uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
  LOG_SCOPE_FUNCTION(INFO);
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i))
        && (memProperties.memoryTypes[i].propertyFlags & properties)
               == properties)
    {
      return i;
    }
  }
  throw nebulaException("Vulkan: failed to find suitable memory type");
}

void graphics::pipeline::createDescriptorSetLayout()
{
  LOG_SCOPE_FUNCTION(1);
  VkDescriptorSetLayoutBinding uboLayoutBinding {};
  uboLayoutBinding.binding         = 0;
  uboLayoutBinding.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags      = VK_SHADER_STAGE_VERTEX_BIT;
  VkDescriptorSetLayoutCreateInfo layoutInfo {};
  layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings    = &uboLayoutBinding;
  if (vkCreateDescriptorSetLayout(
          _device, &layoutInfo, nullptr, &_descriptorSetLayout)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create descriptor set layout");
  }
}

void graphics::createUniformBuffers()
{
  LOG_SCOPE_FUNCTION(INFO);
  VkDeviceSize bufferSize = sizeof(uniformBufferObject);
  _uniformBuffers.resize(_swapChainImages.size());
  _uniformBuffersMemory.resize(_swapChainImages.size());
  for (size_t i = 0; i < _swapChainImages.size(); i++) {
    createBuffer(bufferSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        _uniformBuffers[i],
        _uniformBuffersMemory[i]);
  }
}

void graphics::createDescriptorPool()
{
  LOG_SCOPE_FUNCTION(INFO);
  VkDescriptorPoolSize poolSize {};
  poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = static_cast<uint32_t>(_swapChainImages.size());
  VkDescriptorPoolCreateInfo poolInfo {};
  poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes    = &poolSize;
  poolInfo.maxSets       = static_cast<uint32_t>(_swapChainImages.size());
  if (vkCreateDescriptorPool(
          _logicalDevice, &poolInfo, nullptr, &_descriptorPool)
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to create descriptor pool");
  }
}

void graphics::createDescriptorSets()
{
  LOG_SCOPE_FUNCTION(INFO);
  std::vector<VkDescriptorSetLayout> layouts(
      _swapChainImages.size(), _pipeline->_descriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo {};
  allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool     = _descriptorPool;
  allocInfo.descriptorSetCount = static_cast<uint32_t>(_swapChainImages.size());
  allocInfo.pSetLayouts        = layouts.data();
  _descriptorSets.resize(_swapChainImages.size());
  if (vkAllocateDescriptorSets(
          _logicalDevice, &allocInfo, _descriptorSets.data())
      != VK_SUCCESS)
  {
    throw nebulaException("Vulkan: failed to allocate descriptor sets");
  }
  for (size_t i = 0; i < _swapChainImages.size(); i++) {
    VkDescriptorBufferInfo bufferInfo {};
    bufferInfo.buffer = _uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range  = sizeof(uniformBufferObject);
    VkWriteDescriptorSet descriptorWrite {};
    descriptorWrite.sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet          = _descriptorSets[i];
    descriptorWrite.dstBinding      = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo     = &bufferInfo;
    vkUpdateDescriptorSets(_logicalDevice, 1, &descriptorWrite, 0, nullptr);
  }
}

} // namespace nebula
