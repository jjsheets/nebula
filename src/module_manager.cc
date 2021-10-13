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
    nebula::moduleManager modManager;
    WHEN("a non-local mod path is added")
    {
      modManager.addModulePath("data", false);
      THEN("getModulePaths returns a path in the current working directory")
      {
        REQUIRE(modManager.getModulePaths()
                == (const std::string &)(std::filesystem::current_path())
                       + "/data");
      }
    }
    WHEN("a local mod path is added")
    {
      modManager.addModulePath("MyGame", true);
      THEN("getModulePaths returns a path relative to the user's game folder")
      {
        REQUIRE(modManager.getModulePaths()
                == sago::getSaveGamesFolder2() + "/MyGame");
      }
    }
  }
}

namespace nebula {

void moduleManager::addModulePath(const std::string &path, bool user)
{
  LOG_SCOPE_FUNCTION(INFO);
  if (user) {
    _modPaths.emplace(
        _modPaths.end(), sago::getSaveGamesFolder2() + "/" + path, user);
  } else {
    _modPaths.emplace(_modPaths.end(),
        (const std::string &)(std::filesystem::current_path()) + "/" + path,
        user);
  }
  LOG_S(INFO) << "Added Module Path: " << _modPaths.back()._path;
}

std::string moduleManager::getModulePaths()
{
  std::string pathList("");
  std::for_each(_modPaths.begin(),
      _modPaths.end(),
      [&](const moduleManager::modPath &v) { pathList.append(v._path); });
  return pathList;
}

void moduleManager::modPath::loadManifests(moduleManager &manager)
{
  for (const auto &entry : std::filesystem::directory_iterator(_path))
    if (entry.is_directory()) {
      // TODO: move this into modules constructor
      manager._modules.emplace_back(entry.path().string());
    }
}

} // namespace nebula
