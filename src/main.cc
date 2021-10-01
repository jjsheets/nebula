// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MAIN_CC
#define NEBULA_MAIN_CC

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char *description)
{
  std::cerr << "Error: " << description << std::endl;
}

static void key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(int argc, char *argv[])
{
  if (!glfwInit()) {
    return -1;
  }
  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(640, 480, "Nebula", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 0;
  }
  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

#endif // NEBULA_MAIN_CC
