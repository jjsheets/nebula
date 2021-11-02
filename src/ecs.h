// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_ECS_H
#define NEBULA_ECS_H

extern "C" {
#include "sqlite3.h"
}

namespace nebula {

class ecs {
private:
  sqlite3 *_db;

public:
  ecs();
  ~ecs();
};

} // namespace nebula

#endif // NEBULA_ECS_H
