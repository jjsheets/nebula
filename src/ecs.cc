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
    REQUIRE_NOTHROW(nebula::ecs state);
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
    throw sqliteException(res);
  }
  sqlite3_stmt *insertEntityTable;
  if (sqlite3_prepare_v2(_db,
          "CREATE TABLE entity (entity INTEGER PRIMARY KEY, id TEXT UNIQUE);",
          -1,
          &insertEntityTable,
          nullptr)
      != SQLITE_OK)
  {
    throw sqliteException(_db);
  }
  if (sqlite3_step(insertEntityTable) != SQLITE_DONE) {
    throw sqliteException(_db);
  }
  LOG_S(INFO) << "SQL: Entity table created";
  if (sqlite3_finalize(insertEntityTable) != SQLITE_OK) {
    throw sqliteException(_db);
  }
}

ecs::~ecs()
{
  LOG_SCOPE_FUNCTION(INFO);
  sqlite3_close(_db);
}

} // namespace nebula
