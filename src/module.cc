// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

// Exception includes
#include "exceptions.h"
#include "module.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

// Logging system includes
#include "loguru.hpp"

// Unit Testing includes
#include "doctest.h"

#ifndef DOCTEST_CONFIG_DISABLE

SCENARIO("class module")
{
  GIVEN("an invalid module path/module path without manifest")
  {
    auto path = "test/invalid";
    THEN("exception is thrown when creating a module with the path")
    {
      REQUIRE_THROWS(nebula::module(path));
    }
  }
  GIVEN("a module with a manifest that lacks a module section")
  {
    auto path = "test/no-module-section";
    THEN("exception is thrown when creating a module with the path")
    {
      REQUIRE_THROWS(nebula::module(path));
    }
  }
  GIVEN("a module with a manifest that lacks an identifier")
  {
    auto path = "test/no-identifier";
    THEN("exception is thrown when creating a module with the path")
    {
      REQUIRE_THROWS(nebula::module(path));
    }
  }
  GIVEN("a module with a manifest that lacks a name")
  {
    auto path = "test/no-name";
    auto mod  = nebula::module(path);
    THEN("the name should equal the module's identifier")
    {
      REQUIRE(mod.name() == mod.identifier());
    }
  }
  GIVEN("a valid module")
  {
    auto path = "test/valid-module";
    auto mod  = nebula::module(path, true);
    WHEN("loadModule() is called")
    {
      mod.loadModule();
      THEN("generated SQL should be correct")
      {
        REQUIRE(mod.getComponentSQL("test")
                == "CREATE TABLE test (entity INTEGER PRIMARY KEY REFERENCES "
                   "entity(entity) ON DELETE CASCADE, test_int INTEGER, "
                   "test_num REAL, test_txt TEXT);");
        REQUIRE(mod.getSystemSQL("update_location")
                == "UPDATE location SET theta = old.theta + mobile.rotation, x "
                   "= old.x - sin(old.theta) * mobile.vel * deltaT(), y = "
                   "old.y + cos(old.theta) * mobile.vel * deltaT() FROM "
                   "location AS old JOIN mobile USING (entity) WHERE vel > "
                   "0.0;");
      }
    }
  }
}

#endif

#define NEBULA_SEMVER "0.1.0"

namespace nebula {

module::module(const std::string &path, bool shouldLoad)
    : _rootPath(path), _load(shouldLoad)
{
  YAML::Node manifest;
  try {
    manifest = YAML::LoadFile(path + "/manifest.yml");
  } catch (YAML::BadFile &e) {
    LOG_S(ERROR) << "Manifest file is missing or corrupt: " << path
                 << "/manifest.yml";
    throw;
  }
  if (!manifest["module"]) {
    throw nebulaException(
        "Manifest file lacks module section: " + path + "/manifest.yml");
  }
  if (!manifest["module"]["id"]) {
    throw nebulaException(
        "Manifest file lacks identifier: " + path + "/manifest.yml");
  }
  _identifier = manifest["module"]["id"].as<std::string>();
  _name = manifest["module"]["name"].as<std::string, std::string>(_identifier);
  std::string _tags
      = manifest["module"]["tags"].as<std::string, std::string>("");
  if (manifest["module"]["dependencies"]) {
    for (const auto &iterator : manifest["module"]["dependencies"]) {
      _dependencies.emplace_back(iterator.as<std::string>());
    }
  }
  if (manifest["module"]["include"]) {
    for (const auto &iterator : manifest["module"]["include"]) {
      _includes.emplace_back(iterator.as<std::string>());
    }
  }
}

module::module(const module &other)
    : _rootPath(other._rootPath), _load(other._load),
      _identifier(other._identifier), _name(other._name), _tags(other._tags),
      _dependencies(other._dependencies)
{
}

module::~module() { }

void module::loadModule()
{
  for (auto fileName : _includes) {
    YAML::Node include;
    auto filePath = _rootPath + "/" + fileName;
    LOG_S(INFO) << "Loading " << filePath;
    try {
      include = YAML::LoadFile(filePath);
    } catch (YAML::BadFile &e) {
      LOG_S(ERROR) << "Included file is missing or corrupt: " << filePath;
      throw;
    }
    if (include["components"]) {
      if (!include["components"].IsMap()) {
        throw nebulaException("Invalid components section: not type Map");
      }
      for (auto componentNode = include["components"].begin();
           componentNode != include["components"].end();
           ++componentNode)
      {
        loadComponent(
            componentNode->first.as<std::string>(), componentNode->second);
      }
    }
    if (include["systems"]) {
      if (!include["systems"].IsMap()) {
        throw nebulaException("Invalid systems section: not type Map");
      }
      for (auto systemNode = include["systems"].begin();
           systemNode != include["systems"].end();
           ++systemNode)
      {
        loadSystem(systemNode->first.as<std::string>(), systemNode->second);
      }
    }
  }
}

void module::loadComponent(std::string key, YAML::Node &component)
{
  if (!component.IsMap()) {
    throw nebulaException("Invalid component " + key + ": not type Map");
  }
  std::string sql = "CREATE TABLE " + key
                  + " (entity INTEGER PRIMARY KEY "
                    "REFERENCES entity(entity) ON DELETE CASCADE";
  for (auto value = component.begin(); value != component.end(); ++value) {
    const std::string &dtype = value->second.as<std::string>();
    sql += ", " + value->first.as<std::string>() + " ";
    for (auto &c : dtype)
      sql += std::toupper(c);
  }
  sql += ");";
  _componentSQL[key] = sql;
}

void module::loadSystem(std::string key, YAML::Node &system)
{
  if (!system.IsMap()) {
    throw nebulaException("Invalid system " + key + ": not type Map");
  }
  if (system["update"]) {
    YAML::Node update = system["update"];
    if (!update["component"]) {
      throw nebulaException("Invalid system " + key + ": no component field");
    }
    if (!update["set"]) {
      throw nebulaException("Invalid system " + key + ": no set field");
    }
    YAML::Node set = update["set"];
    std::string sql
        = "UPDATE " + update["component"].as<std::string>() + " SET ";
    for (auto value = set.begin(); value != set.end(); ++value) {
      if (value != set.begin()) {
        sql += ", ";
      }
      sql += value->first.as<std::string>() + " = "
           + value->second.as<std::string>();
    }
    if (update["entity_join"]) {
      YAML::Node join = update["entity_join"];
      sql += " FROM ";
      for (auto value = join.begin(); value != join.end(); ++value) {
        if (value != join.begin()) {
          sql += " JOIN ";
        }
        auto component = value->second.as<std::string>();
        auto name      = value->first.as<std::string>();
        if (component != name) {
          sql += component + " AS ";
        }
        sql += name;
        if (value != join.begin()) {
          sql += " USING (entity)";
        }
      }
    }
    if (update["require"]) {
      YAML::Node require = update["require"];
      sql += " WHERE ";
      for (auto value = require.begin(); value != require.end(); ++value) {
        if (value != require.begin()) {
          sql += ", ";
        }
        sql += value->first.as<std::string>() + " "
             + value->second.as<std::string>();
      }
    }
    sql += ";";
    _systemSQL[key] = sql;
    return;
  }
  throw nebulaException("No valid system configuration found for " + key);
}

const std::string module::getComponentSQL(const std::string &component)
{
  if (_componentSQL.count(component) > 0)
    return _componentSQL.at(component);
  throw nebulaException(
      "Component '" + component + "' does not exist in module '" + _name + "'");
}

const std::string module::getSystemSQL(const std::string &system)
{
  if (_systemSQL.count(system) > 0)
    return _systemSQL.at(system);
  throw nebulaException(
      "System '" + system + "' does not exist in module '" + _name + "'");
}

} // namespace nebula
