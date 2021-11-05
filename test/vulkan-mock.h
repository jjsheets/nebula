// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_TEST_VULKAN_MOCK_H
#define NEBULA_TEST_VULKAN_MOCK_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <cassert>
#include <queue>

class vulkan_mock {
private:
  GLFWwindow *testWindow;
  VkInstance testVkInstance;
  VkDebugUtilsMessengerEXT testDUMEXT;
  VkSurfaceKHR testSurface;
  VkPhysicalDevice testPhysDev;
  VkDevice testLogDev;
  VkQueue testCombinedQueue;
  VkQueue testGraphicsQueue;
  VkQueue testPresentQueue;
  VkSwapchainKHR testSwapChain;
  VkRenderPass testRenderPass;
  VkPipelineLayout testPipeLayout;
  VkCommandPool testCmdPool;
  const char *testSurfaceExt;
  const char *testSurfaceExts[2];
  void *windowUP = nullptr;
  uint32_t testSwapChainImageCount;
  uint32_t testImageCur  = 0;
  uint32_t testDrawImage = 0;
  std::stack<std::unique_ptr<trompeloeil::expectation>> expectations;
  int width;
  int height;
  std::string title;
  int _glfwShouldClose = GLFW_FALSE;
  std::function<void(GLFWwindow *, int, int, int, int)> _keyCB;
  std::queue<std::function<void()>> _evBuffer;
  uint64_t _loopCount             = 0;
  uint64_t _loopMax               = 60 * 5;
  bool _separateQueues            = false;
  bool _useAlternateSurfaceFormat = false;

  void fillSurfCaps(VkSurfaceCapabilitiesKHR &caps);
  void fillSurfFmt(VkSurfaceFormatKHR &fmt);
  void fillPhysDevProps(VkPhysicalDeviceProperties &props);
  void fillDevExtProps(VkExtensionProperties &props);
  void fillPhysDevMemProps(VkPhysicalDeviceMemoryProperties &props);
  void genImgHandles(uint32_t n, VkImage *images);
  void makePipelines(
      uint32_t n, const VkGraphicsPipelineCreateInfo *info, VkPipeline *p);
  void fillCmdBuffer(uint32_t n, VkCommandBuffer *bufs);
  bool validCmdBuffer(VkCommandBuffer &b);
  void nextImage(uint32_t *n);
  void fillFamilyProperties(VkQueueFamilyProperties c[]);

public:
  static vulkan_mock *&instance()
  {
    static vulkan_mock *obj = nullptr;
    return obj;
  }
  vulkan_mock()
  {
    assert(instance() == nullptr);
    instance() = this;
  }
  ~vulkan_mock()
  {
    assert(instance() == this);
    instance() = nullptr;
  }
  vulkan_mock(const vulkan_mock &) = delete;
  vulkan_mock &operator=(const vulkan_mock &) = delete;
  void mockGraphics();
  void simKeyPress(int key, int mod, bool release);
  void maxLoop(uint64_t m);
  void pollEvents();
  void enableSeparateQueues();
  void enableAltSurfaceFormat();

  MAKE_MOCK2(glfwSetWindowShouldClose, void(GLFWwindow *, int));
  MAKE_MOCK0(glfwPollEvents, void());
  MAKE_MOCK1(glfwWindowShouldClose, int(GLFWwindow *));
  MAKE_MOCK3(glfwGetWindowSize, void(GLFWwindow *, int *, int *));
  MAKE_MOCK5(glfwCreateWindow,
      GLFWwindow *(int, int, const char *, GLFWmonitor *, GLFWwindow *));
  MAKE_MOCK4(glfwCreateWindowSurface,
      VkResult(VkInstance,
          GLFWwindow *,
          const VkAllocationCallbacks *,
          VkSurfaceKHR *));
  MAKE_MOCK1(glfwDestroyWindow, void(GLFWwindow *));
  MAKE_MOCK1(glfwGetError, int(const char **));
  MAKE_MOCK3(glfwGetFramebufferSize, void(GLFWwindow *, int *, int *));
  MAKE_MOCK1(glfwGetRequiredInstanceExtensions, const char **(uint32_t *));
  MAKE_MOCK0(glfwGetVersionString, const char *());
  MAKE_MOCK2(glfwGetWindowAttrib, int(GLFWwindow *, int));
  MAKE_MOCK1(glfwGetWindowUserPointer, void *(GLFWwindow *));
  MAKE_MOCK0(glfwInit, int());
  MAKE_MOCK2(glfwSetFramebufferSizeCallback,
      GLFWframebuffersizefun(GLFWwindow *, GLFWframebuffersizefun));
  MAKE_MOCK2(glfwSetKeyCallback, GLFWkeyfun(GLFWwindow *, GLFWkeyfun));
  MAKE_MOCK2(glfwSetWindowUserPointer, void(GLFWwindow *, void *));
  MAKE_MOCK0(glfwTerminate, void());
  MAKE_MOCK0(glfwWaitEvents, void());
  MAKE_MOCK2(glfwWindowHint, void(int, int));
  MAKE_MOCK6(vkAcquireNextImageKHR,
      VkResult(VkDevice,
          VkSwapchainKHR,
          uint64_t,
          VkSemaphore,
          VkFence,
          uint32_t *f));
  MAKE_MOCK3(vkAllocateCommandBuffers,
      VkResult(
          VkDevice, const VkCommandBufferAllocateInfo *, VkCommandBuffer *));
  MAKE_MOCK2(vkBeginCommandBuffer,
      VkResult(VkCommandBuffer, const VkCommandBufferBeginInfo *));
  MAKE_MOCK3(vkCmdBeginRenderPass,
      void(VkCommandBuffer, const VkRenderPassBeginInfo *, VkSubpassContents));
  MAKE_MOCK3(vkCmdBindPipeline,
      void(VkCommandBuffer, VkPipelineBindPoint, VkPipeline));
  MAKE_MOCK5(
      vkCmdDraw, void(VkCommandBuffer, uint32_t, uint32_t, uint32_t, uint32_t));
  MAKE_MOCK1(vkCmdEndRenderPass, void(VkCommandBuffer));
  MAKE_MOCK4(vkCreateCommandPool,
      VkResult(VkDevice,
          const VkCommandPoolCreateInfo *,
          const VkAllocationCallbacks *,
          VkCommandPool *));
  MAKE_MOCK4(vkCreateDevice,
      VkResult(VkPhysicalDevice,
          const VkDeviceCreateInfo *,
          const VkAllocationCallbacks *,
          VkDevice *));
  MAKE_MOCK4(vkCreateFence,
      VkResult(VkDevice,
          const VkFenceCreateInfo *,
          const VkAllocationCallbacks *,
          VkFence *));
  MAKE_MOCK4(vkCreateFramebuffer,
      VkResult(VkDevice,
          const VkFramebufferCreateInfo *,
          const VkAllocationCallbacks *,
          VkFramebuffer *));
  MAKE_MOCK6(vkCreateGraphicsPipelines,
      VkResult(VkDevice,
          VkPipelineCache,
          uint32_t,
          const VkGraphicsPipelineCreateInfo *,
          const VkAllocationCallbacks *,
          VkPipeline *));
  MAKE_MOCK4(vkCreateImageView,
      VkResult(VkDevice,
          const VkImageViewCreateInfo *,
          const VkAllocationCallbacks *,
          VkImageView *));
  MAKE_MOCK3(vkCreateInstance,
      VkResult(const VkInstanceCreateInfo *,
          const VkAllocationCallbacks *,
          VkInstance *));
  MAKE_MOCK4(vkCreatePipelineLayout,
      VkResult(VkDevice,
          const VkPipelineLayoutCreateInfo *,
          const VkAllocationCallbacks *,
          VkPipelineLayout *));
  MAKE_MOCK4(vkCreateRenderPass,
      VkResult(VkDevice,
          const VkRenderPassCreateInfo *,
          const VkAllocationCallbacks *,
          VkRenderPass *));
  MAKE_MOCK4(vkCreateSemaphore,
      VkResult(VkDevice,
          const VkSemaphoreCreateInfo *,
          const VkAllocationCallbacks *,
          VkSemaphore *));
  MAKE_MOCK4(vkCreateShaderModule,
      VkResult(VkDevice,
          const VkShaderModuleCreateInfo *,
          const VkAllocationCallbacks *,
          VkShaderModule *));
  MAKE_MOCK4(vkCreateSwapchainKHR,
      VkResult(VkDevice,
          const VkSwapchainCreateInfoKHR *,
          const VkAllocationCallbacks *,
          VkSwapchainKHR *));
  MAKE_MOCK3(vkDestroyCommandPool,
      void(VkDevice, VkCommandPool, const VkAllocationCallbacks *));
  MAKE_MOCK2(vkDestroyDevice, void(VkDevice, const VkAllocationCallbacks *));
  MAKE_MOCK3(
      vkDestroyFence, void(VkDevice, VkFence, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyFramebuffer,
      void(VkDevice, VkFramebuffer, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyImageView,
      void(VkDevice, VkImageView, const VkAllocationCallbacks *));
  MAKE_MOCK2(
      vkDestroyInstance, void(VkInstance, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyPipeline,
      void(VkDevice, VkPipeline, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyPipelineLayout,
      void(VkDevice, VkPipelineLayout, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyRenderPass,
      void(VkDevice, VkRenderPass, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroySemaphore,
      void(VkDevice, VkSemaphore, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroyShaderModule,
      void(VkDevice, VkShaderModule, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroySurfaceKHR,
      void(VkInstance, VkSurfaceKHR, const VkAllocationCallbacks *));
  MAKE_MOCK3(vkDestroySwapchainKHR,
      void(VkDevice, VkSwapchainKHR, const VkAllocationCallbacks *));
  MAKE_MOCK1(vkDeviceWaitIdle, VkResult(VkDevice));
  MAKE_MOCK1(vkEndCommandBuffer, VkResult(VkCommandBuffer));
  MAKE_MOCK4(vkEnumerateDeviceExtensionProperties,
      VkResult(
          VkPhysicalDevice, const char *, uint32_t *, VkExtensionProperties *));
  MAKE_MOCK2(vkEnumerateInstanceLayerProperties,
      VkResult(uint32_t *, VkLayerProperties *));
  MAKE_MOCK3(vkEnumeratePhysicalDevices,
      VkResult(VkInstance, uint32_t *, VkPhysicalDevice *));
  MAKE_MOCK4(vkFreeCommandBuffers,
      void(VkDevice, VkCommandPool, uint32_t, const VkCommandBuffer *));
  MAKE_MOCK4(vkGetDeviceQueue, void(VkDevice, uint32_t, uint32_t, VkQueue *));
  MAKE_MOCK2(
      vkGetInstanceProcAddr, PFN_vkVoidFunction(VkInstance, const char *));
  MAKE_MOCK2(vkGetPhysicalDeviceMemoryProperties,
      void(VkPhysicalDevice, VkPhysicalDeviceMemoryProperties *));
  MAKE_MOCK2(vkGetPhysicalDeviceProperties,
      void(VkPhysicalDevice, VkPhysicalDeviceProperties *));
  MAKE_MOCK3(vkGetPhysicalDeviceQueueFamilyProperties,
      void(VkPhysicalDevice, uint32_t *, VkQueueFamilyProperties *));
  MAKE_MOCK3(vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
      VkResult(
          VkPhysicalDevice a, VkSurfaceKHR b, VkSurfaceCapabilitiesKHR *c));
  MAKE_MOCK4(vkGetPhysicalDeviceSurfaceFormatsKHR,
      VkResult(
          VkPhysicalDevice, VkSurfaceKHR, uint32_t *, VkSurfaceFormatKHR *));
  MAKE_MOCK4(vkGetPhysicalDeviceSurfacePresentModesKHR,
      VkResult(VkPhysicalDevice, VkSurfaceKHR, uint32_t *, VkPresentModeKHR *));
  MAKE_MOCK4(vkGetPhysicalDeviceSurfaceSupportKHR,
      VkResult(VkPhysicalDevice, uint32_t, VkSurfaceKHR, VkBool32 *));
  MAKE_MOCK4(vkGetSwapchainImagesKHR,
      VkResult(VkDevice, VkSwapchainKHR, uint32_t *, VkImage *));
  MAKE_MOCK2(vkQueuePresentKHR, VkResult(VkQueue, const VkPresentInfoKHR *));
  MAKE_MOCK4(vkQueueSubmit,
      VkResult(VkQueue, uint32_t, const VkSubmitInfo *, VkFence));
  MAKE_MOCK3(vkResetFences, VkResult(VkDevice, uint32_t, const VkFence *));
  MAKE_MOCK5(vkWaitForFences,
      VkResult(VkDevice, uint32_t, const VkFence *, VkBool32, uint64_t));
  MAKE_MOCK4(vkCreateDebugUtilsMessengerEXT,
      VkResult(VkInstance,
          const VkDebugUtilsMessengerCreateInfoEXT *,
          const VkAllocationCallbacks *,
          VkDebugUtilsMessengerEXT *));
  MAKE_MOCK3(vkDestroyDebugUtilsMessengerEXT,
      void(
          VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *));
};

extern vulkan_mock vkMock;

#endif // NEBULA_TEST_VULKAN_MOCK_H
