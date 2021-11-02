// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module.h"
#include <stdexcept>
#include <algorithm>
#include "yaml-cpp/yaml.h"

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
    LOG_S(ERROR) << "Manifest file is missing or corrupt:";
    LOG_S(ERROR) << "  " << path << "/manifest.yml";
    throw;
  }
  if (!manifest["module"]) {
    LOG_S(ERROR) << "Manifest file lacks module section:";
    LOG_S(ERROR) << "  " << path << "/manifest.yml";
    throw std::runtime_error("Manifest file lacks module section");
  }
  if (!manifest["module"]["id"]) {
    LOG_S(ERROR) << "Manifest file lacks identifier:";
    LOG_S(ERROR) << "  " << path << "/manifest.yml";
    throw std::runtime_error("Manifest file lacks identifier");
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

} // namespace nebula
