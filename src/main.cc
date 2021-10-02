// This document is licensed according to the LGPL v2.1 license
// Consult the LICENSE file in the root project directory for details

#ifndef NEBULA_MAIN_CC
#define NEBULA_MAIN_CC

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine.h"

int main(int argc, char *argv[])
{
  nebula::engine nebulaEngine;
  nebulaEngine.loop();
  return 0;
}

#endif // NEBULA_MAIN_CC
