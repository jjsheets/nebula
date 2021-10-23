// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module_manager.h"

#include <algorithm>

// Unit Testing includes
#include "doctest.h"

// Logging system includes
#include "loguru.hpp"

// Folder Handling includes
#include "platform_folders.h"

#ifndef DOCTEST_CONFIG_DISABLE

namespace nebula {

std::string moduleManager::getModulePaths()
{
  std::string pathList("");
  std::for_each(_modPaths.begin(),
      _modPaths.end(),
      [&](const moduleManager::modPath &v) { pathList.append(v._path); });
  return pathList;
}

} // namespace nebula

#endif

SCENARIO("class moduleManager" * doctest::may_fail())
{
  GIVEN("a moduleManager object")
  {
    nebula::moduleManager modManager;
    WHEN("a core mod path is added")
    {
      modManager.addModulePath("samples/asteroids/data", false);
      THEN("getModulePaths returns a path in the current working directory")
      {
        REQUIRE(modManager.getModulePaths()
                == (const std::string &)(std::filesystem::current_path())
                       + "/samples/asteroids/data");
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
        _modPaths.end(), sago::getSaveGamesFolder2() + "/" + path, user, *this);
  } else {
    _modPaths.emplace(_modPaths.end(),
        std::filesystem::current_path().native() + "/" + path,
        user,
        *this);
  }
  LOG_S(INFO) << "Added Module Path: " << _modPaths.back()._path;
}

void moduleManager::modPath::loadManifests(moduleManager &manager)
{
  for (const auto &entry : std::filesystem::directory_iterator(_path))
    if (entry.is_directory()) {
      manager._modules.emplace_back(entry.path().string());
    }
}

void moduleManager::resolve()
{
  std::for_each(_modules.begin(), _modules.end(), [&](auto &mod) {
    if (mod.load() && _resolved.find(mod.identifier()) == _resolved.end()) {
      resolve(mod);
    }
  });
}

void moduleManager::resolve(module &mod)
{
  if (_resolved.find(mod.identifier()) != _resolved.end()) {
    return;
  }
  _unresolved.insert(mod.identifier());
  std::for_each(
      mod.dependencies().begin(), mod.dependencies().end(), [&](auto &dep) {
        if (_resolved.find(dep) == _resolved.end()) {
          if (_unresolved.find(dep) != _resolved.end()) {
            throw std::runtime_error("Circular dependency detected!");
          }
          resolve(_moduleMap.at(dep));
        }
      });
  _resolved.insert(mod.identifier());
  _unresolved.erase(mod.identifier());
}

} // namespace nebula
