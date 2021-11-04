
#include <doctest.h>
#include <doctest/trompeloeil.hpp>
#include "../test/vulkan-mock.h"

// Logging system includes
#include "loguru.hpp"
#include <cstring>

using trompeloeil::_;

extern "C" {

void glfwSetWindowShouldClose(GLFWwindow *a, int b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwSetWindowShouldClose(a, b);
}

void glfwPollEvents()
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwPollEvents();
}

int glfwWindowShouldClose(GLFWwindow *a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwWindowShouldClose(a);
}

void glfwGetWindowSize(GLFWwindow *a, int *b, int *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwGetWindowSize(a, b, c);
}

GLFWwindow *glfwCreateWindow(
    int a, int b, const char *c, GLFWmonitor *d, GLFWwindow *e)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwCreateWindow(a, b, c, d, e);
}

VkResult glfwCreateWindowSurface(VkInstance a,
    GLFWwindow *b,
    const VkAllocationCallbacks *c,
    VkSurfaceKHR *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwCreateWindowSurface(a, b, c, d);
}

void glfwDestroyWindow(GLFWwindow *a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwDestroyWindow(a);
}

int glfwGetError(const char **a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwGetError(a);
}

void glfwGetFramebufferSize(GLFWwindow *a, int *b, int *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwGetFramebufferSize(a, b, c);
}

const char **glfwGetRequiredInstanceExtensions(uint32_t *a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwGetRequiredInstanceExtensions(a);
}

const char *glfwGetVersionString()
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwGetVersionString();
}

int glfwGetWindowAttrib(GLFWwindow *a, int b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwGetWindowAttrib(a, b);
}

void *glfwGetWindowUserPointer(GLFWwindow *a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwGetWindowUserPointer(a);
}

int glfwInit()
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwInit();
}

GLFWframebuffersizefun glfwSetFramebufferSizeCallback(
    GLFWwindow *a, GLFWframebuffersizefun b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwSetFramebufferSizeCallback(a, b);
}

GLFWkeyfun glfwSetKeyCallback(GLFWwindow *a, GLFWkeyfun b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->glfwSetKeyCallback(a, b);
}

void glfwSetWindowUserPointer(GLFWwindow *a, void *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwSetWindowUserPointer(a, b);
}

void glfwTerminate()
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwTerminate();
}

void glfwWaitEvents()
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwWaitEvents();
}

void glfwWindowHint(int a, int b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->glfwWindowHint(a, b);
}

VkResult vkAcquireNextImageKHR(VkDevice a,
    VkSwapchainKHR b,
    uint64_t c,
    VkSemaphore d,
    VkFence e,
    uint32_t *f)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkAcquireNextImageKHR(a, b, c, d, e, f);
}

VkResult vkAllocateCommandBuffers(
    VkDevice a, const VkCommandBufferAllocateInfo *b, VkCommandBuffer *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkAllocateCommandBuffers(a, b, c);
}

VkResult vkBeginCommandBuffer(
    VkCommandBuffer a, const VkCommandBufferBeginInfo *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkBeginCommandBuffer(a, b);
}

void vkCmdBeginRenderPass(
    VkCommandBuffer a, const VkRenderPassBeginInfo *b, VkSubpassContents c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkCmdBeginRenderPass(a, b, c);
}

void vkCmdBindPipeline(VkCommandBuffer a, VkPipelineBindPoint b, VkPipeline c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCmdBindPipeline(a, b, c);
}

void vkCmdDraw(
    VkCommandBuffer a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkCmdDraw(a, b, c, d, e);
}

void vkCmdEndRenderPass(VkCommandBuffer a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkCmdEndRenderPass(a);
}

VkResult vkCreateCommandPool(VkDevice a,
    const VkCommandPoolCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkCommandPool *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateCommandPool(a, b, c, d);
}

VkResult vkCreateDevice(VkPhysicalDevice a,
    const VkDeviceCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkDevice *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateDevice(a, b, c, d);
}

VkResult vkCreateFence(VkDevice a,
    const VkFenceCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkFence *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateFence(a, b, c, d);
}

VkResult vkCreateFramebuffer(VkDevice a,
    const VkFramebufferCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkFramebuffer *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateFramebuffer(a, b, c, d);
}

VkResult vkCreateGraphicsPipelines(VkDevice a,
    VkPipelineCache b,
    uint32_t c,
    const VkGraphicsPipelineCreateInfo *d,
    const VkAllocationCallbacks *e,
    VkPipeline *f)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateGraphicsPipelines(a, b, c, d, e, f);
}

VkResult vkCreateImageView(VkDevice a,
    const VkImageViewCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkImageView *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateImageView(a, b, c, d);
}

VkResult vkCreateInstance(const VkInstanceCreateInfo *a,
    const VkAllocationCallbacks *b,
    VkInstance *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateInstance(a, b, c);
}

VkResult vkCreatePipelineLayout(VkDevice a,
    const VkPipelineLayoutCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkPipelineLayout *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreatePipelineLayout(a, b, c, d);
}

VkResult vkCreateRenderPass(VkDevice a,
    const VkRenderPassCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkRenderPass *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateRenderPass(a, b, c, d);
}

VkResult vkCreateSemaphore(VkDevice a,
    const VkSemaphoreCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkSemaphore *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateSemaphore(a, b, c, d);
}

VkResult vkCreateShaderModule(VkDevice a,
    const VkShaderModuleCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkShaderModule *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateShaderModule(a, b, c, d);
}

VkResult vkCreateSwapchainKHR(VkDevice a,
    const VkSwapchainCreateInfoKHR *b,
    const VkAllocationCallbacks *c,
    VkSwapchainKHR *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateSwapchainKHR(a, b, c, d);
}

void vkDestroyCommandPool(
    VkDevice a, VkCommandPool b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyCommandPool(a, b, c);
}

void vkDestroyDevice(VkDevice a, const VkAllocationCallbacks *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyDevice(a, b);
}

void vkDestroyFence(VkDevice a, VkFence b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyFence(a, b, c);
}

void vkDestroyFramebuffer(
    VkDevice a, VkFramebuffer b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyFramebuffer(a, b, c);
}

void vkDestroyImageView(
    VkDevice a, VkImageView b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyImageView(a, b, c);
}

void vkDestroyInstance(VkInstance a, const VkAllocationCallbacks *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyInstance(a, b);
}

void vkDestroyPipeline(VkDevice a, VkPipeline b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyPipeline(a, b, c);
}

void vkDestroyPipelineLayout(
    VkDevice a, VkPipelineLayout b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyPipelineLayout(a, b, c);
}

void vkDestroyRenderPass(
    VkDevice a, VkRenderPass b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyRenderPass(a, b, c);
}

void vkDestroySemaphore(
    VkDevice a, VkSemaphore b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroySemaphore(a, b, c);
}

void vkDestroyShaderModule(
    VkDevice a, VkShaderModule b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroyShaderModule(a, b, c);
}

void vkDestroySurfaceKHR(
    VkInstance a, VkSurfaceKHR b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroySurfaceKHR(a, b, c);
}

void vkDestroySwapchainKHR(
    VkDevice a, VkSwapchainKHR b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkDestroySwapchainKHR(a, b, c);
}

VkResult vkDeviceWaitIdle(VkDevice a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkDeviceWaitIdle(a);
}

VkResult vkEndCommandBuffer(VkCommandBuffer a)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkEndCommandBuffer(a);
}

VkResult vkEnumerateDeviceExtensionProperties(
    VkPhysicalDevice a, const char *b, uint32_t *c, VkExtensionProperties *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkEnumerateDeviceExtensionProperties(a, b, c, d);
}

VkResult vkEnumerateInstanceLayerProperties(uint32_t *a, VkLayerProperties *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkEnumerateInstanceLayerProperties(a, b);
}

VkResult vkEnumeratePhysicalDevices(
    VkInstance a, uint32_t *b, VkPhysicalDevice *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkEnumeratePhysicalDevices(a, b, c);
}

void vkFreeCommandBuffers(
    VkDevice a, VkCommandPool b, uint32_t c, const VkCommandBuffer *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkFreeCommandBuffers(a, b, c, d);
}

void vkGetDeviceQueue(VkDevice a, uint32_t b, uint32_t c, VkQueue *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkGetDeviceQueue(a, b, c, d);
}

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance a, const char *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetInstanceProcAddr(a, b);
}

void vkGetPhysicalDeviceMemoryProperties(
    VkPhysicalDevice a, VkPhysicalDeviceMemoryProperties *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkGetPhysicalDeviceMemoryProperties(a, b);
}

void vkGetPhysicalDeviceProperties(
    VkPhysicalDevice a, VkPhysicalDeviceProperties *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkGetPhysicalDeviceProperties(a, b);
}

void vkGetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice a, uint32_t *b, VkQueueFamilyProperties *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  vkMock->vkGetPhysicalDeviceQueueFamilyProperties(a, b, c);
}

VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, VkSurfaceCapabilitiesKHR *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a, b, c);
}

VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, uint32_t *c, VkSurfaceFormatKHR *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetPhysicalDeviceSurfaceFormatsKHR(a, b, c, d);
}

VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, uint32_t *c, VkPresentModeKHR *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetPhysicalDeviceSurfacePresentModesKHR(a, b, c, d);
}

VkResult vkGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice a, uint32_t b, VkSurfaceKHR c, VkBool32 *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetPhysicalDeviceSurfaceSupportKHR(a, b, c, d);
}

VkResult vkGetSwapchainImagesKHR(
    VkDevice a, VkSwapchainKHR b, uint32_t *c, VkImage *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkGetSwapchainImagesKHR(a, b, c, d);
}

VkResult vkQueuePresentKHR(VkQueue a, const VkPresentInfoKHR *b)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkQueuePresentKHR(a, b);
}

VkResult vkQueueSubmit(VkQueue a, uint32_t b, const VkSubmitInfo *c, VkFence d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkQueueSubmit(a, b, c, d);
}

VkResult vkResetFences(VkDevice a, uint32_t b, const VkFence *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkResetFences(a, b, c);
}

VkResult vkWaitForFences(
    VkDevice a, uint32_t b, const VkFence *c, VkBool32 d, uint64_t e)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkWaitForFences(a, b, c, d, e);
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance a,
    const VkDebugUtilsMessengerCreateInfoEXT *b,
    const VkAllocationCallbacks *c,
    VkDebugUtilsMessengerEXT *d)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkCreateDebugUtilsMessengerEXT(a, b, c, d);
}
void vkDestroyDebugUtilsMessengerEXT(
    VkInstance a, VkDebugUtilsMessengerEXT b, const VkAllocationCallbacks *c)
{
  auto vkMock = vulkan_mock::instance();
  assert(vkMock);
  return vkMock->vkDestroyDebugUtilsMessengerEXT(a, b, c);
}
}

void vulkan_mock::fillSurfCaps(VkSurfaceCapabilitiesKHR &caps)
{
  LOG_SCOPE_FUNCTION(1);
  caps.minImageCount           = 1;
  caps.maxImageCount           = 0;
  caps.currentExtent.width     = 2560;
  caps.currentExtent.height    = 1440;
  caps.minImageExtent.width    = 1;
  caps.minImageExtent.height   = 1;
  caps.maxImageExtent.width    = 2560;
  caps.maxImageExtent.height   = 1440;
  caps.maxImageArrayLayers     = 1;
  caps.supportedTransforms     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  caps.currentTransform        = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
  caps.supportedCompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  caps.supportedUsageFlags     = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
}

void vulkan_mock::fillSurfFmt(VkSurfaceFormatKHR &fmt)
{
  fmt.format     = VK_FORMAT_B8G8R8A8_SRGB;
  fmt.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
}

void vulkan_mock::fillPhysDevProps(VkPhysicalDeviceProperties &props)
{
  props.apiVersion    = VK_MAKE_VERSION(1, 2, 155);
  props.driverVersion = VK_MAKE_VERSION(1, 0, 0);
  props.vendorID      = 0;
  props.deviceID      = 0;
  // Provide a discrete gpu, even though this is a test fixture
  props.deviceType = VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  std::strcpy(props.deviceName, "nebula test device");
  for (int i = 0; i < VK_UUID_SIZE; i++) {
    props.pipelineCacheUUID[i] = 0;
  }
  // ignoring limits and sparseProperties for now.
}

void vulkan_mock::fillDevExtProps(VkExtensionProperties &props)
{
  std::strcpy(props.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void vulkan_mock::fillPhysDevMemProps(VkPhysicalDeviceMemoryProperties &props)
{
  props.memoryTypeCount              = 1;
  props.memoryHeapCount              = 1;
  props.memoryTypes[0].propertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
                                     | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                     | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  props.memoryTypes[0].heapIndex = 0;
  props.memoryHeaps[0].size      = 8ULL * 1024ULL * 1024ULL * 1024ULL;
  props.memoryHeaps[0].flags     = VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
}

void vulkan_mock::genImgHandles(uint32_t n, VkImage *images)
{
  for (uint32_t i = 0; i < n; i++) {
    images[i] = reinterpret_cast<VkImage>(testImageCur++);
  }
}

void vulkan_mock::makePipelines(
    uint32_t n, const VkGraphicsPipelineCreateInfo *info, VkPipeline *p)
{
  for (uint32_t i = 0; i < n; i++) {
    p[i] = reinterpret_cast<VkPipeline>(
        const_cast<VkGraphicsPipelineCreateInfo *>(info));
  }
}

void vulkan_mock::fillCmdBuffer(uint32_t n, VkCommandBuffer *bufs)
{
  for (uint32_t i = 0; i < n; i++) {
    bufs[i] = reinterpret_cast<VkCommandBuffer>(0xE01 + i);
  }
}

bool vulkan_mock::validCmdBuffer(VkCommandBuffer &b)
{
  return (0xE00 < reinterpret_cast<uint64_t>(b))
      && (reinterpret_cast<uint64_t>(b) < 0xF00);
}

void vulkan_mock::nextImage(uint32_t *n)
{
  *n = testDrawImage++;
  testDrawImage %= testSwapChainImageCount;
}

void vulkan_mock::mockGraphics()
{
  testWindow         = reinterpret_cast<GLFWwindow *>(0x400);
  testVkInstance     = reinterpret_cast<VkInstance>(0x500);
  testDUMEXT         = reinterpret_cast<VkDebugUtilsMessengerEXT>(0x600);
  testSurface        = reinterpret_cast<VkSurfaceKHR>(0x700);
  testPhysDev        = reinterpret_cast<VkPhysicalDevice>(0x800);
  testLogDev         = reinterpret_cast<VkDevice>(0x900);
  testCombinedQueue  = reinterpret_cast<VkQueue>(0xA00);
  testSwapChain      = reinterpret_cast<VkSwapchainKHR>(0xB00);
  testRenderPass     = reinterpret_cast<VkRenderPass>(0xC00);
  testPipeLayout     = reinterpret_cast<VkPipelineLayout>(0xD00);
  testCmdPool        = reinterpret_cast<VkCommandPool>(0xE00);
  testSurfaceExt     = "VK_KHR_surface";
  testSurfaceExts[0] = testSurfaceExt;
  testSurfaceExts[1] = nullptr;
  expectations.push(
      NAMED_ALLOW_CALL(*this, glfwGetVersionString()).RETURN("GLFW Mocked"));
  expectations.push(NAMED_REQUIRE_CALL(*this, glfwInit()).RETURN(GLFW_TRUE));
  expectations.push(NAMED_ALLOW_CALL(*this, glfwWindowHint(_, _)));
  expectations.push(
      NAMED_REQUIRE_CALL(*this, glfwCreateWindow(_, _, _, nullptr, nullptr))
          .SIDE_EFFECT(width = _1)
          .SIDE_EFFECT(height = _2)
          .SIDE_EFFECT(title = _3)
          .RETURN(testWindow));
  expectations.push(
      NAMED_REQUIRE_CALL(*this, glfwSetWindowUserPointer(testWindow, _))
          .LR_SIDE_EFFECT(windowUP = _2));
  expectations.push(
      NAMED_REQUIRE_CALL(*this, glfwSetFramebufferSizeCallback(testWindow, _))
          .RETURN(nullptr));
  expectations.push(NAMED_REQUIRE_CALL(*this, glfwSetKeyCallback(testWindow, _))
                        .RETURN(nullptr));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkEnumerateInstanceLayerProperties(_, nullptr))
          .SIDE_EFFECT(*_1 = 1)
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkEnumerateInstanceLayerProperties(_, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(std::strcpy(
                            _2->layerName, "VK_LAYER_KHRONOS_validation"))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, glfwGetRequiredInstanceExtensions(_))
          .SIDE_EFFECT(*_1 = 1)
          .LR_RETURN((testSurfaceExts)));
  expectations.push(NAMED_REQUIRE_CALL(*this, vkCreateInstance(_, nullptr, _))
                        .SIDE_EFFECT(*_3 = testVkInstance)
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_REQUIRE_CALL(*this,
          vkGetInstanceProcAddr(testVkInstance,
              trompeloeil::re("^vkCreateDebugUtilsMessengerEXT$")))
          .RETURN((PFN_vkVoidFunction)(&::vkCreateDebugUtilsMessengerEXT)));
  expectations.push(NAMED_REQUIRE_CALL(
      *this, vkCreateDebugUtilsMessengerEXT(testVkInstance, _, nullptr, _))
                        .SIDE_EFFECT(*_4 = testDUMEXT)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_REQUIRE_CALL(
      *this, glfwCreateWindowSurface(testVkInstance, testWindow, nullptr, _))
                        .SIDE_EFFECT(*_4 = testSurface)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkEnumeratePhysicalDevices(testVkInstance, _, nullptr))
                        .SIDE_EFFECT(*_2 = 1)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkEnumeratePhysicalDevices(testVkInstance, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(*_3 = testPhysDev)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkGetPhysicalDeviceQueueFamilyProperties(testPhysDev, _, nullptr))
                        .SIDE_EFFECT(*_2 = 1));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceQueueFamilyProperties(
          testPhysDev, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(_3->queueFlags = VK_QUEUE_GRAPHICS_BIT));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfaceSupportKHR(testPhysDev, _, testSurface, _))
                        .SIDE_EFFECT(*_4 = true)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfaceCapabilitiesKHR(testPhysDev, testSurface, _))
                        .SIDE_EFFECT(fillSurfCaps(*_3))
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfaceFormatsKHR(
          testPhysDev, testSurface, _, nullptr))
                        .SIDE_EFFECT(*_3 = 1)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfaceFormatsKHR(
          testPhysDev, testSurface, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(fillSurfFmt(*_4))
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfacePresentModesKHR(
          testPhysDev, testSurface, _, nullptr))
                        .SIDE_EFFECT(*_3 = 1)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetPhysicalDeviceSurfacePresentModesKHR(
          testPhysDev, testSurface, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(*_4 = VK_PRESENT_MODE_FIFO_KHR)
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkGetPhysicalDeviceProperties(testPhysDev, _))
          .SIDE_EFFECT(fillPhysDevProps(*_2)));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkEnumerateDeviceExtensionProperties(testPhysDev, nullptr, _, nullptr))
                        .SIDE_EFFECT(*_3 = 1)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkEnumerateDeviceExtensionProperties(
          testPhysDev, nullptr, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(fillDevExtProps(*_4))
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkGetPhysicalDeviceMemoryProperties(testPhysDev, _))
                        .SIDE_EFFECT(fillPhysDevMemProps(*_2)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateDevice(testPhysDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = testLogDev)
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkGetDeviceQueue(testLogDev, 0u, 0u, _))
          .SIDE_EFFECT(*_4 = testCombinedQueue));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateSwapchainKHR(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(testSwapChainImageCount = _2->minImageCount)
          .SIDE_EFFECT(*_4 = testSwapChain)
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkGetSwapchainImagesKHR(testLogDev, testSwapChain, _, nullptr))
                        .SIDE_EFFECT(*_3 = testSwapChainImageCount)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this,
      vkGetSwapchainImagesKHR(
          testLogDev, testSwapChain, _, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(genImgHandles(testSwapChainImageCount, _4))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this,
          vkCreateImageView(testLogDev, _, nullptr, trompeloeil::ne(nullptr)))
          .SIDE_EFFECT(*_4 = reinterpret_cast<VkImageView>(_2->image))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateRenderPass(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = testRenderPass)
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateShaderModule(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = reinterpret_cast<const VkShaderModule>(
                           const_cast<uint32_t *>(_2->pCode)))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreatePipelineLayout(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = testPipeLayout)
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkCreateGraphicsPipelines(testLogDev, nullptr, _, _, nullptr, _))
                        .SIDE_EFFECT(makePipelines(_3, _4, _6))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyShaderModule(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateFramebuffer(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = reinterpret_cast<VkFramebuffer>(
                           const_cast<VkImageView>(_2->pAttachments[0])))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateCommandPool(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = testCmdPool)
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkAllocateCommandBuffers(testLogDev, _, _))
          .SIDE_EFFECT(fillCmdBuffer(_2->commandBufferCount, _3))
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this, vkBeginCommandBuffer(_, _))
                        .WITH(validCmdBuffer(_1))
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this, vkCmdBeginRenderPass(_, _, _))
                        .WITH(validCmdBuffer(_1)));
  expectations.push(NAMED_ALLOW_CALL(*this, vkCmdBindPipeline(_, _, _))
                        .WITH(validCmdBuffer(_1)));
  expectations.push(NAMED_ALLOW_CALL(*this, vkCmdDraw(_, _, _, _, _))
                        .WITH(validCmdBuffer(_1)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCmdEndRenderPass(_)).WITH(validCmdBuffer(_1)));
  expectations.push(NAMED_ALLOW_CALL(*this, vkEndCommandBuffer(_))
                        .WITH(validCmdBuffer(_1))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateSemaphore(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = reinterpret_cast<VkSemaphore>(_4))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkCreateFence(testLogDev, _, nullptr, _))
          .SIDE_EFFECT(*_4 = reinterpret_cast<VkFence>(_4))
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this, glfwGetWindowSize(testWindow, _, _))
                        .SIDE_EFFECT(*_2 = width)
                        .SIDE_EFFECT(*_3 = height));
  expectations.push(
      NAMED_ALLOW_CALL(*this, glfwGetWindowAttrib(testWindow, GLFW_DECORATED))
          .RETURN(true));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDeviceWaitIdle(testLogDev)).RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroySemaphore(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyFence(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyFramebuffer(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkFreeCommandBuffers(testLogDev, _, _, _)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyPipeline(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyPipelineLayout(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyRenderPass(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyImageView(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroySwapchainKHR(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyCommandPool(testLogDev, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyDevice(testLogDev, nullptr)));
  expectations.push(
      NAMED_REQUIRE_CALL(*this,
          vkGetInstanceProcAddr(testVkInstance,
              trompeloeil::re("^vkDestroyDebugUtilsMessengerEXT$")))
          .RETURN((PFN_vkVoidFunction)(&::vkDestroyDebugUtilsMessengerEXT)));
  expectations.push(NAMED_ALLOW_CALL(
      *this, vkDestroyDebugUtilsMessengerEXT(testVkInstance, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroySurfaceKHR(testVkInstance, _, nullptr)));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkDestroyInstance(testVkInstance, nullptr)));
  expectations.push(NAMED_ALLOW_CALL(*this, glfwDestroyWindow(testWindow)));
  expectations.push(NAMED_ALLOW_CALL(*this, glfwTerminate()));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkWaitForFences(testLogDev, _, _, _, _))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkAcquireNextImageKHR(testLogDev, _, _, _, _, _))
          .SIDE_EFFECT(nextImage(_6))
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(*this, vkResetFences(testLogDev, _, _))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkQueueSubmit(testCombinedQueue, _, _, _))
          .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(*this, vkQueuePresentKHR(testCombinedQueue, _))
          .RETURN(VK_SUCCESS));
}
