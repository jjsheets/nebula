// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_MANAGER_H
#define NEBULA_MODULE_MANAGER_H

#include <string>
#include <vector>
#include "module.h"

namespace nebula {

class moduleManager {
private:
  struct modPath {
    std::string _path;
    bool _autoLoad;
    bool _local;

    modPath(const std::string &path, bool autoLoad, bool local)
        : _path(path), _autoLoad(autoLoad), _local(local)
    {
    }
  };

  std::vector<modPath> _modPaths;
  std::string _localFolderName;

public:
  moduleManager(const std::string &localFolderName)
      : _localFolderName(localFolderName)
  {
  }
  ~moduleManager() { }

  void addModulePath(const std::string &path, bool autoLoad, bool local);
  std::string getModulePaths();
};

} // namespace nebula

#endif // NEBULA_MODULE_MANAGER_H
