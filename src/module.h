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
  std::string _identifier;
  std::string _name;
  std::string _tags;
  std::vector<std::string> _dependencies;

public:
  module(const std::string &path);
  module(const module &other);
  module() { }
  ~module();

  const std::string &identifier()
  {
    return _identifier;
  }
};

} // namespace nebula

#endif // NEBULA_MODULE_H
