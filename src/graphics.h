// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_GRAPHICS_H
#define NEBULA_GRAPHICS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>
#include <string>

namespace nebula {

class graphics {
public:
  class pipeline {
  private:
    VkDevice _device;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _graphicsPipeline;

    static std::vector<char> readFile(const std::string &filename);
    VkShaderModule createShaderModule(const std::vector<char> &code);

  public:
    pipeline(VkDevice device,
        const std::string &vertShader,
        const std::string &fragShader,
        VkExtent2D &swapChainExtent,
        VkRenderPass renderPass);
    ~pipeline();
    operator VkPipeline()
    {
      return _graphicsPipeline;
    }
  };

private:
  uint32_t _width;
  uint32_t _height;
  GLFWwindow *_window;
  VkInstance _instance;
  bool _useValidationLayers;
  VkDebugUtilsMessengerEXT _debugMessenger;
  VkPhysicalDevice _physicalDevice;
  VkDevice _logicalDevice;
  VkQueue _graphicsQueue;
  VkQueue _presentQueue;
  VkSurfaceKHR _surface;
  VkSwapchainKHR _swapChain;
  std::vector<VkImage> _swapChainImages;
  VkFormat _swapChainImageFormat;
  VkExtent2D _swapChainExtent;
  std::vector<VkImageView> _swapChainImageViews;
  std::vector<VkFramebuffer> _swapChainFramebuffers;
  pipeline *_pipeline;
  VkRenderPass _renderPass;
  VkCommandPool _commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;
  std::vector<VkSemaphore> _renderFinished;
  std::vector<VkSemaphore> _imageAvailable;
  std::vector<VkFence> _inFlightFence;
  std::vector<VkFence> _inFlightImage;
  size_t _currentFrame;
  bool _framebufferResized;

  const std::vector<const char *> _validationLayers
      = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> _deviceExtensions
      = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  const size_t _maxFramesInFlight = 2;

  void initGLFW(GLFWkeyfun keyCallback);
  void createVulkanInstance();
  bool checkValidationLayerSupport();
  std::vector<const char *> getRequiredExtensions();
  void setValidationCallback();
  void setDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void pickPhysicalDevice();
  bool deviceIsSuitable(VkPhysicalDevice device);
  void createLogicalDevice();
  void createSurface();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
  void createSwapChain();
  void recreateSwapChain();
  void cleanupSwapChain();
  void createImageViews();
  void logMemoryType(VkMemoryType &mType);
  void logMemoryHeap(VkMemoryHeap &mHeap, uint32_t i);
  void logPhysicalDevice();
  void createRenderPass();
  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();
  void createSyncObjects();

  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
      void *pUserData);
  static void framebufferResizeCallback(
      GLFWwindow *window, int width, int height);

  struct queueFamilyIndices {
    std::optional<uint32_t> _graphicsFamily;
    std::optional<uint32_t> _presentFamily;

    queueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool isComplete();
  };

  struct swapChainSupportDetails {
    VkSurfaceCapabilitiesKHR _capabilities;
    std::vector<VkSurfaceFormatKHR> _formats;
    std::vector<VkPresentModeKHR> _presentModes;

    swapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool isSuitable();
  };

public:
  graphics(uint32_t width,
      uint32_t height,
      GLFWkeyfun keyCallback,
      bool useValidationLayers);
  ~graphics();
  void drawFrame();
  operator GLFWwindow *()
  {
    return _window;
  }
  void setClose(bool close);
};

} // namespace nebula

#endif // NEBULA_GRAPHICS_H
