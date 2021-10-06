// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ENGINE_CC
#define NEBULA_ENGINE_CC

#include "engine.h"

// Exception includes
#include "exceptions.h"

// Unit Testing includes
#include "doctest.h"

// Logging system includes
#include "loguru.hpp"

// Lua scripting includes
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

SCENARIO("class engine" * doctest::may_fail())
{
  GIVEN("an engine object with only command line arguments")
  {
    char argv0[] = "nebula";
    char argv1[] = "-v";
    char argv2[] = "OFF";
    char *argv[] = {argv0, argv1, argv2, NULL};
    nebula::engine testEngine(3, argv);
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

engine::engine(int argc, char *argv[])
{
  loguru::init(argc, argv);
  loguru::add_file("log/verbose.log", loguru::Truncate, loguru::Verbosity_MAX);
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

  LOG_S(INFO) << "GLFW " << glfwGetVersionString();
  if (!glfwInit()) {
    LOG_S(ERROR) << "Could not initialize GLFW: Platform does not meet minimum "
                    "requirements for GLFW initialization";
    throw glfwException();
  }
  LOG_S(INFO) << "GLFW library loaded";
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  _window = glfwCreateWindow(640, 480, "Nebula", NULL, NULL);
  if (!_window) {
    LOG_S(ERROR) << "GLFW: Window creation failed";
    glfwTerminate();
    throw glfwException();
  }
  LOG_S(INFO) << "GLFW: Game window created (640x480)";
  glfwSetKeyCallback(_window, engine::_keyCallback);
  _engine = this;
}

engine::~engine()
{
  LOG_SCOPE_FUNCTION(INFO);
  if (_window) {
    glfwDestroyWindow(_window);
    LOG_S(INFO) << "GLFW: Game window destroyed";
  }
  glfwTerminate();
  LOG_S(INFO) << "GLFW: Terminated";
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
  glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

void engine::loop()
{
  LOG_SCOPE_FUNCTION(INFO);
  while (!glfwWindowShouldClose(_window)) {
    glfwPollEvents();
  }
}

} // namespace nebula

#endif // NEBULA_ENGINE_CC
