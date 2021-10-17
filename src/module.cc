// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module.h"
#include <stdexcept>
#include <algorithm>
#include "yaml-cpp/yaml.h"

// Logging system includes
#include "loguru.hpp"

#define NEBULA_SEMVER "0.1.0"

namespace nebula {

module::module(const std::string &path, bool shouldLoad)
    : _rootPath(path), _load(shouldLoad)
{
  YAML::Node manifest = YAML::LoadFile(path + "/manifest.yml");
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
  _name       = manifest["module"]["name"].as<std::string, std::string>("");
  std::string _tags
      = manifest["module"]["tags"].as<std::string, std::string>("");
  if (manifest["module"]["dependencies"]) {
    // TODO: handle dependency tracking
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
