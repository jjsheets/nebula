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

SCENARIO("class moduleManager")
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
                == (std::filesystem::current_path().u8string()
                    + "/samples/asteroids/data"));
      }
    }
  }
  GIVEN("a moduleManager object")
  {
    nebula::moduleManager modManager;
    WHEN("a local mod path is added")
    {
      modManager.addModulePath("MyGame", true);
      THEN("getModulePaths returns a path relative to the user's game folder")
      {
        REQUIRE(modManager.getModulePaths()
                == (sago::getSaveGamesFolder2() + "/MyGame"));
      }
    }
  }
  GIVEN("a moduleManager object")
  {
    nebula::moduleManager modManager;
    WHEN("a mod path with circular dependencies is added and all mods loaded")
    {
      modManager.addModulePath("test/circular-deps", false);
      for (auto &mod : modManager.modules()) {
        mod.load(true);
      }
      THEN("resolveModules should throw an error")
      {
        REQUIRE_THROWS(modManager.resolveModules());
      }
    }
  }
}

#endif

namespace nebula {

void moduleManager::addModulePath(const std::string &path, bool user)
{
  LOG_SCOPE_FUNCTION(INFO);
  if (user) {
    _modPaths.emplace(
        _modPaths.end(), sago::getSaveGamesFolder2() + "/" + path, user, *this);
  } else {
    _modPaths.emplace(_modPaths.end(),
        std::filesystem::current_path().u8string() + "/" + path,
        user,
        *this);
  }
  LOG_S(INFO) << "Added Module Path: " << _modPaths.back()._path;
}

void moduleManager::modPath::loadManifests(moduleManager &manager)
{
  LOG_SCOPE_FUNCTION(INFO);
  for (const auto &entry : std::filesystem::directory_iterator(_path))
    if (entry.is_directory()) {
      module &m = manager._modules.emplace_back(entry.path().string());
      manager._moduleMap[m.identifier()] = m;
    }
}

void moduleManager::resolveModules()
{
  LOG_SCOPE_FUNCTION(INFO);
  for (auto &mod : _modules) {
    LOG_S(INFO) << "Module: " << mod.identifier();
    if (mod.load() && _resolved.find(mod.identifier()) == _resolved.end()) {
      resolveModule(mod);
    }
  };
}

void moduleManager::resolveModule(module &mod)
{
  LOG_SCOPE_FUNCTION(INFO);
  _unresolved.insert(mod.identifier());
  for (auto &dep : mod.dependencies()) {
    LOG_S(INFO) << "Dependency: " << dep;
    if (_resolved.find(dep) == _resolved.end()) {
      LOG_S(INFO) << "  Dependency not already resolved";
      if (_unresolved.find(dep) != _unresolved.end()) {
        LOG_S(ERROR) << "Circular module dependency detected";
        throw std::runtime_error("Circular module dependency detected");
      }
      LOG_S(INFO) << "  No Circular dependency detected, resolving " << dep;
      resolveModule(_moduleMap[dep]);
    }
  };
  _resolved.insert(mod.identifier());
  _unresolved.erase(mod.identifier());
}

} // namespace nebula
