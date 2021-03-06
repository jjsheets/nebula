// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "engine.h"

// Exception includes
#include "exceptions.h"

// Logging system includes
#include "loguru.hpp"

// Lua scripting includes
// extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
//}

#ifndef DOCTEST_CONFIG_DISABLE
  #include "doctest.h"
  #include <doctest/trompeloeil.hpp>
  #include "../test/vulkan-mock.h"

SCENARIO("class engine")
{
  GIVEN("an engine object")
  {
    // Set up the expectations for this test in a mock object
    vulkanMock vkMock;
    vkMock.mockGraphics();
    vkMock.simKeyPress(GLFW_KEY_ESCAPE, 0, true);

    nebula::engine testEngine;
    THEN("it should have a 1600 x 900 GLFW window with a border")
    {
      int width, height;
      glfwGetWindowSize(testEngine.getWindowPointer(), &width, &height);
      REQUIRE(width == 1600);
      REQUIRE(height == 900);
      REQUIRE(
          glfwGetWindowAttrib(testEngine.getWindowPointer(), GLFW_DECORATED));
    }
  }
  GIVEN("an engine object")
  {
    vulkanMock vkMock;
    vkMock.mockGraphics();
    vkMock.maxLoop(10);

    nebula::engine testEngine;
    WHEN("The escape key is pressed")
    {
      vkMock.simKeyPress(GLFW_KEY_ESCAPE, 0, true);
      THEN("it should stop its loop")
      {
        testEngine.loop();
      }
    }
  }
  GIVEN("an engine object")
  {
    vulkanMock vkMock;
    vkMock.mockGraphics();
    vkMock.maxLoop(10);

    nebula::engine testEngine;
    WHEN("exit() is called on the engine")
    {
      testEngine.exit();
      THEN("it should stop its loop")
      {
        testEngine.loop();
      }
    }
  }
}
#endif

namespace nebula {

engine *engine::_engine;

static void *_luaAlloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
  if (nsize == 0) {
    free(ptr);
    return NULL;
  } else {
    return realloc(ptr, nsize);
  }
}

static void _luaWarnFunction(void *ud, const char *message, int tocont)
{
  LOG_S(WARNING) << message;
}

static int _luaPanic(lua_State *L)
{
  const char *msg = lua_tostring(L, -1);
  if (!msg) {
    msg = "error object is not a string";
  }
  LOG_S(ERROR) << "unprotected error in call to Lua API (" << msg << ")";
  throw std::runtime_error(msg);
}

engine::engine()
{
  LOG_SCOPE_FUNCTION(INFO);

  LOG_S(INFO) << LUA_RELEASE;
  _luaState = lua_newstate(_luaAlloc, nullptr);
  if (!_luaState) {
    LOG_S(ERROR) << "Could not initialize Lua: Out of memory";
    throw std::runtime_error("Out of memory");
  }
  lua_atpanic(_luaState, &_luaPanic);
  lua_setwarnf(_luaState, _luaWarnFunction, nullptr);
  LOG_S(INFO) << "Lua: scripting library loaded";

  _graphics = new graphics(1600, 900, engine::_keyCallback, true);
  _engine   = this;
}

engine::~engine()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_graphics) {
    delete _graphics;
  }
  lua_close(_luaState);
  LOG_S(INFO) << "Lua: scripting library closed";
}

// static routing function
void engine::_keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
  // No log scope because this just routes the event to the active engine.
  _engine->keyboardEvent(window, key, scancode, action, mods);
}

void engine::keyboardEvent(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    exit();
  }
}

void engine::exit()
{
  LOG_SCOPE_FUNCTION(INFO);
  _graphics->setClose(true);
}

void engine::loop()
{
  LOG_SCOPE_FUNCTION(INFO);
  while (!glfwWindowShouldClose(*_graphics)) {
    glfwPollEvents();
    _graphics->drawFrame();
  }
}

} // namespace nebula
