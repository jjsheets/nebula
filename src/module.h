// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_H
#define NEBULA_MODULE_H

#include <string>
#include <vector>

namespace nebula {

class module {
private:
  std::string _rootPath;
  bool _load;
  std::string _identifier;
  std::string _name;
  std::string _tags;
  std::vector<std::string> _dependencies;

public:
  module(const std::string &path, bool shouldLoad = false);
  module(const module &other);
  module() { }
  ~module();

  const std::string &identifier() const
  {
    return _identifier;
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
};

} // namespace nebula

#endif // NEBULA_MODULE_H
