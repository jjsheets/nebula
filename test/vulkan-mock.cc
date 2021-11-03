
#include <doctest.h>
#include <doctest/trompeloeil.hpp>
#include "../test/vulkan-mock.h"

#include <cstring>

using trompeloeil::_;

vulkan_mock vkMock;

extern "C" {

void glfwSetWindowShouldClose(GLFWwindow *a, int b)
{
  vkMock.glfwSetWindowShouldClose(a, b);
}

void glfwPollEvents()
{
  vkMock.glfwPollEvents();
}

int glfwWindowShouldClose(GLFWwindow *a)
{
  return vkMock.glfwWindowShouldClose(a);
}

void glfwGetWindowSize(GLFWwindow *a, int *b, int *c)
{
  vkMock.glfwGetWindowSize(a, b, c);
}

GLFWwindow *glfwCreateWindow(
    int a, int b, const char *c, GLFWmonitor *d, GLFWwindow *e)
{
  return vkMock.glfwCreateWindow(a, b, c, d, e);
}

VkResult glfwCreateWindowSurface(VkInstance a,
    GLFWwindow *b,
    const VkAllocationCallbacks *c,
    VkSurfaceKHR *d)
{
  return vkMock.glfwCreateWindowSurface(a, b, c, d);
}

void glfwDestroyWindow(GLFWwindow *a)
{
  vkMock.glfwDestroyWindow(a);
}

int glfwGetError(const char **a)
{
  return vkMock.glfwGetError(a);
}

void glfwGetFramebufferSize(GLFWwindow *a, int *b, int *c)
{
  vkMock.glfwGetFramebufferSize(a, b, c);
}

const char **glfwGetRequiredInstanceExtensions(uint32_t *a)
{
  return vkMock.glfwGetRequiredInstanceExtensions(a);
}

const char *glfwGetVersionString()
{
  return vkMock.glfwGetVersionString();
}

int glfwGetWindowAttrib(GLFWwindow *a, int b)
{
  return vkMock.glfwGetWindowAttrib(a, b);
}

void *glfwGetWindowUserPointer(GLFWwindow *a)
{
  return vkMock.glfwGetWindowUserPointer(a);
}

int glfwInit()
{
  return vkMock.glfwInit();
}

GLFWframebuffersizefun glfwSetFramebufferSizeCallback(
    GLFWwindow *a, GLFWframebuffersizefun b)
{
  return vkMock.glfwSetFramebufferSizeCallback(a, b);
}

GLFWkeyfun glfwSetKeyCallback(GLFWwindow *a, GLFWkeyfun b)
{
  return vkMock.glfwSetKeyCallback(a, b);
}

void glfwSetWindowUserPointer(GLFWwindow *a, void *b)
{
  vkMock.glfwSetWindowUserPointer(a, b);
}

void glfwTerminate()
{
  vkMock.glfwTerminate();
}

void glfwWaitEvents()
{
  vkMock.glfwWaitEvents();
}

void glfwWindowHint(int a, int b)
{
  vkMock.glfwWindowHint(a, b);
}

VkResult vkAcquireNextImageKHR(VkDevice a,
    VkSwapchainKHR b,
    uint64_t c,
    VkSemaphore d,
    VkFence e,
    uint32_t *f)
{
  return vkMock.vkAcquireNextImageKHR(a, b, c, d, e, f);
}

VkResult vkAllocateCommandBuffers(
    VkDevice a, const VkCommandBufferAllocateInfo *b, VkCommandBuffer *c)
{
  return vkMock.vkAllocateCommandBuffers(a, b, c);
}

VkResult vkBeginCommandBuffer(
    VkCommandBuffer a, const VkCommandBufferBeginInfo *b)
{
  return vkMock.vkBeginCommandBuffer(a, b);
}

void vkCmdBeginRenderPass(
    VkCommandBuffer a, const VkRenderPassBeginInfo *b, VkSubpassContents c)
{
  vkMock.vkCmdBeginRenderPass(a, b, c);
}

void vkCmdBindPipeline(VkCommandBuffer a, VkPipelineBindPoint b, VkPipeline c)
{
  return vkMock.vkCmdBindPipeline(a, b, c);
}

void vkCmdDraw(
    VkCommandBuffer a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
  vkMock.vkCmdDraw(a, b, c, d, e);
}

void vkCmdEndRenderPass(VkCommandBuffer a)
{
  vkMock.vkCmdEndRenderPass(a);
}

VkResult vkCreateCommandPool(VkDevice a,
    const VkCommandPoolCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkCommandPool *d)
{
  return vkMock.vkCreateCommandPool(a, b, c, d);
}

VkResult vkCreateDevice(VkPhysicalDevice a,
    const VkDeviceCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkDevice *d)
{
  return vkMock.vkCreateDevice(a, b, c, d);
}

VkResult vkCreateFence(VkDevice a,
    const VkFenceCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkFence *d)
{
  return vkMock.vkCreateFence(a, b, c, d);
}

VkResult vkCreateFramebuffer(VkDevice a,
    const VkFramebufferCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkFramebuffer *d)
{
  return vkMock.vkCreateFramebuffer(a, b, c, d);
}

VkResult vkCreateGraphicsPipelines(VkDevice a,
    VkPipelineCache b,
    uint32_t c,
    const VkGraphicsPipelineCreateInfo *d,
    const VkAllocationCallbacks *e,
    VkPipeline *f)
{
  return vkMock.vkCreateGraphicsPipelines(a, b, c, d, e, f);
}

VkResult vkCreateImageView(VkDevice a,
    const VkImageViewCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkImageView *d)
{
  return vkMock.vkCreateImageView(a, b, c, d);
}

VkResult vkCreateInstance(const VkInstanceCreateInfo *a,
    const VkAllocationCallbacks *b,
    VkInstance *c)
{
  return vkMock.vkCreateInstance(a, b, c);
}

VkResult vkCreatePipelineLayout(VkDevice a,
    const VkPipelineLayoutCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkPipelineLayout *d)
{
  return vkMock.vkCreatePipelineLayout(a, b, c, d);
}

VkResult vkCreateRenderPass(VkDevice a,
    const VkRenderPassCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkRenderPass *d)
{
  return vkMock.vkCreateRenderPass(a, b, c, d);
}

VkResult vkCreateSemaphore(VkDevice a,
    const VkSemaphoreCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkSemaphore *d)
{
  return vkMock.vkCreateSemaphore(a, b, c, d);
}

VkResult vkCreateShaderModule(VkDevice a,
    const VkShaderModuleCreateInfo *b,
    const VkAllocationCallbacks *c,
    VkShaderModule *d)
{
  return vkMock.vkCreateShaderModule(a, b, c, d);
}

VkResult vkCreateSwapchainKHR(VkDevice a,
    const VkSwapchainCreateInfoKHR *b,
    const VkAllocationCallbacks *c,
    VkSwapchainKHR *d)
{
  return vkMock.vkCreateSwapchainKHR(a, b, c, d);
}

void vkDestroyCommandPool(
    VkDevice a, VkCommandPool b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyCommandPool(a, b, c);
}

void vkDestroyDevice(VkDevice a, const VkAllocationCallbacks *b)
{
  vkMock.vkDestroyDevice(a, b);
}

void vkDestroyFence(VkDevice a, VkFence b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyFence(a, b, c);
}

void vkDestroyFramebuffer(
    VkDevice a, VkFramebuffer b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyFramebuffer(a, b, c);
}

void vkDestroyImageView(
    VkDevice a, VkImageView b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyImageView(a, b, c);
}

void vkDestroyInstance(VkInstance a, const VkAllocationCallbacks *b)
{
  vkMock.vkDestroyInstance(a, b);
}

void vkDestroyPipeline(VkDevice a, VkPipeline b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyPipeline(a, b, c);
}

void vkDestroyPipelineLayout(
    VkDevice a, VkPipelineLayout b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyPipelineLayout(a, b, c);
}

void vkDestroyRenderPass(
    VkDevice a, VkRenderPass b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyRenderPass(a, b, c);
}

void vkDestroySemaphore(
    VkDevice a, VkSemaphore b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroySemaphore(a, b, c);
}

void vkDestroyShaderModule(
    VkDevice a, VkShaderModule b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroyShaderModule(a, b, c);
}

void vkDestroySurfaceKHR(
    VkInstance a, VkSurfaceKHR b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroySurfaceKHR(a, b, c);
}

void vkDestroySwapchainKHR(
    VkDevice a, VkSwapchainKHR b, const VkAllocationCallbacks *c)
{
  vkMock.vkDestroySwapchainKHR(a, b, c);
}

VkResult vkDeviceWaitIdle(VkDevice a)
{
  return vkMock.vkDeviceWaitIdle(a);
}

VkResult vkEndCommandBuffer(VkCommandBuffer a)
{
  return vkMock.vkEndCommandBuffer(a);
}

VkResult vkEnumerateDeviceExtensionProperties(
    VkPhysicalDevice a, const char *b, uint32_t *c, VkExtensionProperties *d)
{
  return vkMock.vkEnumerateDeviceExtensionProperties(a, b, c, d);
}

VkResult vkEnumerateInstanceLayerProperties(uint32_t *a, VkLayerProperties *b)
{
  return vkMock.vkEnumerateInstanceLayerProperties(a, b);
}

VkResult vkEnumeratePhysicalDevices(
    VkInstance a, uint32_t *b, VkPhysicalDevice *c)
{
  return vkMock.vkEnumeratePhysicalDevices(a, b, c);
}

void vkFreeCommandBuffers(
    VkDevice a, VkCommandPool b, uint32_t c, const VkCommandBuffer *d)
{
  vkMock.vkFreeCommandBuffers(a, b, c, d);
}

void vkGetDeviceQueue(VkDevice a, uint32_t b, uint32_t c, VkQueue *d)
{
  vkMock.vkGetDeviceQueue(a, b, c, d);
}

PFN_vkVoidFunction vkGetInstanceProcAddr(VkInstance a, const char *b)
{
  return vkMock.vkGetInstanceProcAddr(a, b);
}

void vkGetPhysicalDeviceMemoryProperties(
    VkPhysicalDevice a, VkPhysicalDeviceMemoryProperties *b)
{
  vkMock.vkGetPhysicalDeviceMemoryProperties(a, b);
}

void vkGetPhysicalDeviceProperties(
    VkPhysicalDevice a, VkPhysicalDeviceProperties *b)
{
  vkMock.vkGetPhysicalDeviceProperties(a, b);
}

void vkGetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice a, uint32_t *b, VkQueueFamilyProperties *c)
{
  vkMock.vkGetPhysicalDeviceQueueFamilyProperties(a, b, c);
}

VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, VkSurfaceCapabilitiesKHR *c)
{
  return vkMock.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a, b, c);
}

VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, uint32_t *c, VkSurfaceFormatKHR *d)
{
  return vkGetPhysicalDeviceSurfaceFormatsKHR(a, b, c, d);
}

VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice a, VkSurfaceKHR b, uint32_t *c, VkPresentModeKHR *d)
{
  return vkMock.vkGetPhysicalDeviceSurfacePresentModesKHR(a, b, c, d);
}

VkResult vkGetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice a, uint32_t b, VkSurfaceKHR c, VkBool32 *d)
{
  return vkMock.vkGetPhysicalDeviceSurfaceSupportKHR(a, b, c, d);
}

VkResult vkGetSwapchainImagesKHR(
    VkDevice a, VkSwapchainKHR b, uint32_t *c, VkImage *d)
{
  return vkMock.vkGetSwapchainImagesKHR(a, b, c, d);
}

VkResult vkQueuePresentKHR(VkQueue a, const VkPresentInfoKHR *b)
{
  return vkMock.vkQueuePresentKHR(a, b);
}

VkResult vkQueueSubmit(VkQueue a, uint32_t b, const VkSubmitInfo *c, VkFence d)
{
  return vkMock.vkQueueSubmit(a, b, c, d);
}

VkResult vkResetFences(VkDevice a, uint32_t b, const VkFence *c)
{
  return vkMock.vkResetFences(a, b, c);
}

VkResult vkWaitForFences(
    VkDevice a, uint32_t b, const VkFence *c, VkBool32 d, uint64_t e)
{
  return vkMock.vkWaitForFences(a, b, c, d, e);
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance a,
    const VkDebugUtilsMessengerCreateInfoEXT *b,
    const VkAllocationCallbacks *c,
    VkDebugUtilsMessengerEXT *d)
{
  return vkMock.vkCreateDebugUtilsMessengerEXT(a, b, c, d);
}
}

void vulkan_mock::clearExpectations()
{
  while (!expectations.empty()) {
    expectations.pop();
  }
}

void vulkan_mock::mockGraphics()
{
  testWindow         = reinterpret_cast<GLFWwindow *>(400);
  testVkInstance     = reinterpret_cast<VkInstance>(500);
  testDUMEXT         = reinterpret_cast<VkDebugUtilsMessengerEXT>(600);
  testSurface        = reinterpret_cast<VkSurfaceKHR>(700);
  testSurfaceExt     = "VK_KHR_surface";
  testSurfaceExts[0] = testSurfaceExt;
  testSurfaceExts[1] = nullptr;
  expectations.push(
      NAMED_ALLOW_CALL(vkMock, glfwGetVersionString()).RETURN("GLFW Mocked"));
  expectations.push(NAMED_REQUIRE_CALL(vkMock, glfwInit()).RETURN(GLFW_TRUE));
  expectations.push(NAMED_ALLOW_CALL(vkMock, glfwWindowHint(_, _)));
  expectations.push(
      NAMED_REQUIRE_CALL(vkMock, glfwCreateWindow(_, _, _, nullptr, nullptr))
          .SIDE_EFFECT(width = _1)
          .SIDE_EFFECT(height = _2)
          .SIDE_EFFECT(title = _3)
          .RETURN(testWindow));
  expectations.push(
      NAMED_REQUIRE_CALL(vkMock, glfwSetWindowUserPointer(testWindow, _))
          .LR_SIDE_EFFECT(windowUP = _2));
  expectations.push(
      NAMED_REQUIRE_CALL(vkMock, glfwSetFramebufferSizeCallback(testWindow, _))
          .RETURN(nullptr));
  expectations.push(
      NAMED_REQUIRE_CALL(vkMock, glfwSetKeyCallback(testWindow, _))
          .RETURN(nullptr));
  expectations.push(
      NAMED_ALLOW_CALL(vkMock, vkEnumerateInstanceLayerProperties(_, nullptr))
          .SIDE_EFFECT(*_1 = 1)
          .RETURN(VK_SUCCESS));
  expectations.push(NAMED_ALLOW_CALL(
      vkMock, vkEnumerateInstanceLayerProperties(_, trompeloeil::ne(nullptr)))
                        .SIDE_EFFECT(std::strcpy(
                            _2->layerName, "VK_LAYER_KHRONOS_validation"))
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_ALLOW_CALL(vkMock, glfwGetRequiredInstanceExtensions(_))
          .SIDE_EFFECT(*_1 = 1)
          .LR_RETURN((testSurfaceExts)));
  expectations.push(NAMED_REQUIRE_CALL(vkMock, vkCreateInstance(_, nullptr, _))
                        .SIDE_EFFECT(*_3 = testVkInstance)
                        .RETURN(VK_SUCCESS));
  expectations.push(
      NAMED_REQUIRE_CALL(vkMock,
          vkGetInstanceProcAddr(
              testVkInstance, "vkCreateDebugUtilsMessengerEXT"))
          .RETURN((PFN_vkVoidFunction)(&::vkCreateDebugUtilsMessengerEXT)));
  expectations.push(NAMED_REQUIRE_CALL(
      vkMock, vkCreateDebugUtilsMessengerEXT(testVkInstance, _, nullptr, _))
                        .SIDE_EFFECT(*_4 = testDUMEXT)
                        .RETURN(VK_SUCCESS));
  expectations.push(NAMED_REQUIRE_CALL(
      vkMock, glfwCreateWindowSurface(testVkInstance, testWindow, nullptr, _))
                        .SIDE_EFFECT(*_4 = testSurface)
                        .RETURN(VK_SUCCESS));
}
