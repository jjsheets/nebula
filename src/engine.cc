// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ENGINE_CC
#define NEBULA_ENGINE_CC

#include "engine.h"
#include <exception>

#include "doctest.h"

SCENARIO("class engine")
{
  GIVEN("an engine object with no arguments")
  {
    nebula::engine testEngine;
    THEN("it should have a 640 x 480 GLFW window with a border")
    {
      int width, height;
      glfwGetWindowSize(testEngine.getWindowPointer(), &width, &height);
      REQUIRE(width == 640);
      REQUIRE(height == 480);
      REQUIRE(
          glfwGetWindowAttrib(testEngine.getWindowPointer(), GLFW_DECORATED));
    }

#if 0
    // This is for descriptive purposes only, until this can be coded with a
    // proper test
    THEN("it should stop its loop if told to by the OS")
    {
      // When the user hits Alt-F4 or the close button, GLFW handles the events
      // that the OS sends by internally calling glfwSetWindowShouldClose().
      glfwSetWindowShouldClose(testEngine.getWindowPointer(), true);
      testEngine.loop();
    }

    THEN("it should stop its loop if told to by using engine::exit()")
    {
      testEngine.exit();
      testEngine.loop();
    }
#endif
  }
}

namespace nebula {

engine::engine()
{
  if (!glfwInit()) {
    throw std::exception();
  }
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  _window = glfwCreateWindow(640, 480, "Nebula", NULL, NULL);
  if (!_window) {
    glfwTerminate();
    throw std::exception();
  }
}

engine::~engine()
{
  if (_window) {
    glfwDestroyWindow(_window);
  }
  glfwTerminate();
}

void engine::exit()
{
  glfwSetWindowShouldClose(_window, true);
}

void engine::loop()
{
  while (!glfwWindowShouldClose(_window)) {
    glfwPollEvents();
  }
}

} // namespace nebula

#endif // NEBULA_ENGINE_CC
