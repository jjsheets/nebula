// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "module.h"

namespace nebula {

module::module(const std::string &path) : _rootPath(path) { }

module::~module() { }

} // namespace nebula
