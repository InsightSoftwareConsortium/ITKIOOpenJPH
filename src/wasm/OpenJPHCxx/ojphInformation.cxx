// Copyright (c) Chris Hafey.
// SPDX-License-Identifier: MIT

#include "ojphInformation.h"
#include "ojph_defs.h"
#include "ojph_version.h"

char buf[] = ""
  OJPH_INT_TO_STRING(OPENJPH_VERSION_MAJOR) "."
  OJPH_INT_TO_STRING(OPENJPH_VERSION_MINOR) "."
  OJPH_INT_TO_STRING(OPENJPH_VERSION_PATCH);

namespace ojph {
  bool init_cpu_ext_level(int& level);
}

namespace OpenJPH {

std::string getVersion() {
  std::string version = buf;
  return version;
}

unsigned int getSIMDLevel() {
  int level = 0;
  ojph::init_cpu_ext_level(level);
  return level;
}

} // namespace OpenJPH