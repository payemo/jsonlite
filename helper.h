#pragma once

#include <iostream>
#include <cassert>

#define JSONLITE_ASSERT(...) \
    jsonlite::helper::try_assert(__FILE__, __LINE__, #__VA_ARGS__, bool(__VA_ARGS__));

namespace jsonlite
{
  namespace helper
  {
    inline void try_assert(const char *file, int line, const char *expression, bool result) {
      if(!result) {
	std::cerr << "Expression: " << expression << " failed at: " << file << ": " << line << std::endl;
      }
    }
  }
}
