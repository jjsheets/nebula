// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_H
#define NEBULA_MODULE_H

#include <string>
#include <set>
#include <map>
#include <vector>

namespace nebula {

class module {
private:
  static std::set<std::string> _resolved;
  static std::set<std::string> _unresolved;

  std::string _rootPath;
  std::string _identifier;
  std::string _name;
  std::string _tags;
  std::vector<std::string> _dependencies;

  static void resolve(std::map<std::string, module> &modules, module &mod);

public:
  module(const std::string &path);
  module(const module &other);
  module() { }
  ~module();

  static void resolve(std::map<std::string, module> &modules);
};

} // namespace nebula

#endif // NEBULA_MODULE_H
