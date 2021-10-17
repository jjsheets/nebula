// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_MANAGER_H
#define NEBULA_MODULE_MANAGER_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include "module.h"

namespace nebula {

class moduleManager {
private:
  struct modPath {
    std::string _path;
    bool _user;

    modPath(const std::string &path, bool user) : _path(path), _user(user) { }
    void loadManifests(moduleManager &manager);
  };

  std::map<std::string, module> _moduleMap;
  std::vector<module> _modules;
  std::vector<modPath> _modPaths;
  std::set<std::string> _resolved;
  std::set<std::string> _unresolved;

  void resolve(module &mod);

public:
  moduleManager() { }
  ~moduleManager() { }

  void addModulePath(const std::string &path, bool user);
  std::vector<module> &modules();
  void resolve();

#ifndef DOCTEST_CONFIG_DISABLE
  std::string getModulePaths();
#endif
};

} // namespace nebula

#endif // NEBULA_MODULE_MANAGER_H
