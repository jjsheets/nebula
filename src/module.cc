// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module.h"
#include <stdexcept>
#include <algorithm>

#define NEBULA_SEMVER "0.1.0"

namespace nebula {

module::module(const std::string &path) : _rootPath(path)
{
  // read the manifest file for the module
}

module::module(const module &other)
    : _rootPath(other._rootPath), _identifier(other._identifier),
      _name(other._name), _tags(other._tags), _dependencies(other._dependencies)
{
}

module::~module() { }

std::set<std::string> module::_resolved;
std::set<std::string> module::_unresolved;

void module::resolve(std::map<std::string, module> &modules)
{
  // TODO: move this stuff to module_manager class
}

void module::resolve(std::map<std::string, module> &modules, module &mod)
{
  if (_resolved.find(mod._identifier) != _resolved.end()) {
    return;
  }
  _unresolved.insert(mod._identifier);
  std::for_each(
      mod._dependencies.begin(), mod._dependencies.end(), [&](std::string dep) {
        if (_resolved.find(dep) == _resolved.end()) {
          if (_unresolved.find(dep) != _resolved.end()) {
            throw std::runtime_error("Circular dependency detected!");
          }
          resolve(modules, modules.at(dep));
        }
      });
  _resolved.insert(mod._identifier);
  _unresolved.erase(mod._identifier);
}

} // namespace nebula
