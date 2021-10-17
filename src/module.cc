// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module.h"
#include <stdexcept>
#include <algorithm>

#define NEBULA_SEMVER "0.1.0"

namespace nebula {

module::module(const std::string &path, bool shouldLoad)
    : _rootPath(path), _load(shouldLoad)
{
  // read the manifest file for the module
}

module::module(const module &other)
    : _rootPath(other._rootPath), _identifier(other._identifier),
      _name(other._name), _tags(other._tags),
      _dependencies(other._dependencies), _load(other._load)
{
}

module::~module() { }

} // namespace nebula
