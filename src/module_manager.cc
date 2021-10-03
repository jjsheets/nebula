// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module_manager.h"

#include <algorithm>

// Unit Testing includes
#include "doctest.h"

// Logging system includes
#include "loguru.hpp"

SCENARIO("class moduleManager")
{
  GIVEN("a moduleManager object")
  {
    nebula::moduleManager modManager("gameFolder");
    WHEN("a non-local mod path is added")
    {
      modManager.addModulePath("data", true, false);
      THEN("getModulePaths returns a relative file path")
      {
        REQUIRE(modManager.getModulePaths() == "./data");
      }
    }
  }
}

namespace nebula {

void moduleManager::addModulePath(
    const std::string &path, bool autoLoad, bool local)
{
  _modPaths.emplace(_modPaths.end(), path, autoLoad, local);
}

std::string moduleManager::getModulePaths()
{
  std::string pathList("");
  std::for_each(_modPaths.begin(),
      _modPaths.end(),
      [&](const moduleManager::modPath &v) { pathList.append(v._path); });
  return pathList;
}

} // namespace nebula
