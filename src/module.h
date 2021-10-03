// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MODULE_H
#define NEBULA_MODULE_H

#include <string>

namespace nebula {

class module {
private:
  std::string _rootPath;

public:
  module(const std::string &path);
  ~module();
};

} // namespace nebula

#endif // NEBULA_MODULE_H
