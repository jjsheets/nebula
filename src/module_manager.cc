// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module_manager.h"

#include <algorithm>

// Unit Testing includes
#include "doctest.h"

// Logging system includes
#include "loguru.hpp"

// Folder Handling includes
#include <filesystem>
#include "platform_folders.h"

SCENARIO("class moduleManager")
{
  GIVEN("a moduleManager object")
  {
    nebula::moduleManager modManager("gameFolder");
    WHEN("a non-local mod path is added")
    {
      modManager.addModulePath("data", true, false);
      THEN("getModulePaths returns a path in the current working directory")
      {
        REQUIRE(modManager.getModulePaths()
                == (const std::string &)(std::filesystem::current_path())
                       + "/data");
      }
    }
    WHEN("a local mod path is added")
    {
      modManager.addModulePath("MyGame", false, true);
      THEN("getModulePaths returns a path relative to the user's game folder")
      {
        REQUIRE(modManager.getModulePaths()
                == sago::getSaveGamesFolder2() + "/MyGame");
      }
    }
  }
}

namespace nebula {

void moduleManager::addModulePath(
    const std::string &path, bool autoLoad, bool local)
{
  LOG_SCOPE_FUNCTION(INFO);
  if (local) {
    _modPaths.emplace(_modPaths.end(),
        sago::getSaveGamesFolder2() + "/" + path,
        autoLoad,
        local);
  } else {
    _modPaths.emplace(_modPaths.end(),
        (const std::string &)(std::filesystem::current_path()) + "/" + path,
        autoLoad,
        local);
  }
  LOG_S(INFO) << "Added Module Path: " << _modPaths.back()._path
              << (autoLoad ? " (auto load)" : "");
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
