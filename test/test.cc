// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MAIN_CC
#define NEBULA_MAIN_CC

#include "loguru.hpp"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "clipp.h"

int main(int argc, char **argv)
{
  loguru::init(argc, argv);
  loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
  loguru::add_file("test/test.log", loguru::Truncate, loguru::Verbosity_MAX);
  std::cerr << std::flush;
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res = context.run();
  loguru::shutdown();
  bool catLog = false;
  clipp::group cli;
  cli.push_back(clipp::option("--ci-cat-logs").set(catLog));
  clipp::parse(argc, argv, cli);
  if (catLog) {
    if (res != 0) {
      std::ifstream log("test/test.log");
      for (std::string line; std::getline(log, line);) {
        std::cout << line << std::endl;
      }
    }
  }
  if (context.shouldExit())
    return res;
  // If any other code needs to be performed after doctests have completed,
  // this is where it should go.
  return res;
}

#endif // NEBULA_MAIN_CC
