// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#include "ecs.h"

// Exception includes
#include "exceptions.h"

// Unit Testing includes
#include "doctest.h"

// Logging system includes
#include "loguru.hpp"

#ifndef DOCTEST_CONFIG_DISABLE
SCENARIO("class ecs")
{
  GIVEN("an ecs object")
  {
    nebula::ecs state();
  }
}
#endif

namespace nebula {

ecs::ecs() : _db(nullptr)
{
  LOG_SCOPE_FUNCTION(INFO);
  int res = sqlite3_open_v2(
      ":memory:", &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  if (res != SQLITE_OK) {
    sqliteException e(res);
    LOG_S(ERROR) << e.what();
    throw e;
  }
  sqlite3_stmt *insertEntityTable;
  if (sqlite3_prepare_v2(_db,
          "INSERT TABLE entity (entity INTEGER PRIMARY KEY, id TEXT UNIQUE);",
          -1,
          &insertEntityTable,
          nullptr)
      != SQLITE_OK)
  {
    sqliteException e(_db);
    LOG_S(ERROR) << e.what();
    throw e;
  }
  if (sqlite3_step(insertEntityTable) != SQLITE_OK) {
    sqliteException e(_db);
    LOG_S(ERROR) << e.what();
    throw e;
  }
  if (sqlite3_finalize(insertEntityTable) != SQLITE_OK) {
    sqliteException e(_db);
    LOG_S(ERROR) << e.what();
    throw e;
  }
}

ecs::~ecs()
{
  LOG_SCOPE_FUNCTION(INFO);
  sqlite3_close(_db);
}

} // namespace nebula
