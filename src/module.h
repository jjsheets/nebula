// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_H
#define NEBULA_MODULE_H

#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"

namespace nebula {

class module {
private:
  std::string _rootPath;
  bool _load;
  std::string _identifier;
  std::string _name;
  std::string _tags;
  std::vector<std::string> _dependencies;
  std::vector<std::string> _includes;
  std::map<std::string, std::string> _componentSQL;

public:
  module(const std::string &path, bool shouldLoad = false);
  module(const module &other);
  module() { }
  ~module();

  const std::string &identifier() const
  {
    return _identifier;
  }

  const std::string &name() const
  {
    return _name;
  }

  const std::vector<std::string> &dependencies() const
  {
    return _dependencies;
  }

  bool load() const
  {
    return _load;
  }

  void load(bool shouldLoad)
  {
    _load = shouldLoad;
  }

  void loadModule();
  void loadComponent(std::string key, YAML::Node &component);
  const std::string getComponentSQL(const std::string &component);
};

} // namespace nebula

#endif // NEBULA_MODULE_H
