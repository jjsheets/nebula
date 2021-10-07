// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_H
#define NEBULA_MODULE_H

#include <string>
#include <set>
#include <vector>
#include <map>

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

  static void resolve(const module &mod);

public:
  static std::map<std::string, module *> _modules;

  module(const std::string &path);
  ~module();

  static void resolve();
};

} // namespace nebula

#endif // NEBULA_MODULE_H
