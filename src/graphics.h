// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_GRAPHICS_H
#define NEBULA_GRAPHICS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>
#include <string>
#include <array>
#include <glm/glm.hpp>

namespace nebula {

struct vertex {
  glm::vec2 pos;
  glm::vec3 color;
  glm::vec2 texCoord;
  static void getBindingDesc();
  static void getAttributeDesc();
  static VkVertexInputBindingDescription _bindDesc;
  static std::array<VkVertexInputAttributeDescription, 3> _attribDesc;
};

struct uniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

class graphics {
  // public:
private:
  struct pipeline {
    VkDevice _device;
    VkPipelineLayout _pipelineLayout;
    VkPipeline _graphicsPipeline;
    VkPipelineShaderStageCreateInfo _vertShaderStageInfo;
    VkPipelineShaderStageCreateInfo _fragShaderStageInfo;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineViewportStateCreateInfo _viewportState;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo _colorBlending;
    VkPipelineLayoutCreateInfo _pipelineLayoutInfo;
    VkGraphicsPipelineCreateInfo _pipelineInfo;
    VkDescriptorSetLayout _descriptorSetLayout;

    static std::vector<char> readFile(const std::string &filename);
    VkShaderModule createShaderModule(const std::vector<char> &code);
    void setupVertShader(
        VkShaderModule &vertShaderModule, const char *entryPoint);
    void setupFragShader(
        VkShaderModule &fragShaderModule, const char *entryPoint);
    void setupVertexInputState();
    void setupInputAssemblyState();
    void setupViewport(VkExtent2D &swapChainExtent);
    void setupViewportState();
    void setupRasterState();
    void setupMultisampling();
    void setupColorBlend();
    void setupPipelineLayout(VkRenderPass renderPass);
    void createDescriptorSetLayout();

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

  // private:
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
  VkBuffer _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;
  VkBuffer _indexBuffer;
  VkDeviceMemory _indexBufferMemory;
  std::vector<VkBuffer> _uniformBuffers;
  std::vector<VkDeviceMemory> _uniformBuffersMemory;
  VkDescriptorPool _descriptorPool;
  std::vector<VkDescriptorSet> _descriptorSets;
  VkImage _textureImage;
  VkDeviceMemory _textureImageMemory;
  VkImageView _textureImageView;
  VkSampler _textureSampler;

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
  void destroySynchronization();
  void destroyLogicalDevice();
  void destroyVulkanInstance();
  void destroyGLFW();
  void waitForFrame(size_t f);
  void waitForImage(size_t f);
  void submitQueue(uint32_t imageIndex, VkSemaphore signalSemaphores[]);
  void presentQueue(uint32_t imageIndex, VkSemaphore signalSemaphores[]);
  void createBuffer(VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer &buffer,
      VkDeviceMemory &bufferMemory);
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  uint32_t findMemoryType(
      uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void destroyBuffers();
  void updateUniformBuffer(uint32_t currentImage);
  void createDescriptorPool();
  void createDescriptorSets();
  void createTextureImage();
  void createImage(uint32_t width,
      uint32_t height,
      VkFormat format,
      VkImageTiling tiling,
      VkImageUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkImage &image,
      VkDeviceMemory &imageMemory);
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void transitionImageLayout(VkImage image,
      VkFormat format,
      VkImageLayout oldLayout,
      VkImageLayout newLayout);
  void copyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
  void createTextureImageView();
  VkImageView createImageView(VkImage image, VkFormat format);
  void createTextureSampler();

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
  void populateQueueCreateInfos(queueFamilyIndices &queueFamilies,
      std::vector<VkDeviceQueueCreateInfo> &queueCreateInfos);

  struct swapChainSupportDetails {
    VkSurfaceCapabilitiesKHR _capabilities;
    std::vector<VkSurfaceFormatKHR> _formats;
    std::vector<VkPresentModeKHR> _presentModes;

    swapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool isSuitable();
    uint32_t swapChainImageCount();
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
