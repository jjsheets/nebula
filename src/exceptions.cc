// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "exceptions.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Logging system includes
#include "loguru.hpp"

extern "C" {
#include "sqlite3.h"
}

namespace nebula {

glfwException::glfwException() : std::runtime_error("")
{
  const char *_desc;
  _code        = glfwGetError(&_desc);
  _description = _desc;
  LOG_S(ERROR) << _description;
}

glfwException::glfwException(const std::string &logMsg) : std::runtime_error("")
{
  const char *_desc;
  _code        = glfwGetError(&_desc);
  _description = _desc;
  LOG_S(ERROR) << logMsg;
  LOG_S(ERROR) << _description;
}

sqliteException::sqliteException(sqlite3 *db) : std::runtime_error("")
{
  _code        = sqlite3_extended_errcode(db);
  _description = sqlite3_errmsg(db);
  LOG_S(ERROR) << _description;
}

sqliteException::sqliteException(int err) : std::runtime_error("")
{
  _code        = err;
  _description = sqlite3_errmsg(nullptr);
  LOG_S(ERROR) << _description;
}

nebulaException::nebulaException(const std::string &description)
    : std::runtime_error(""), _description(description)
{
  LOG_S(ERROR) << _description;
}

} // namespace nebula
