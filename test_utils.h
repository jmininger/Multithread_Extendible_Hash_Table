#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "extendible_hash.h"

#include <cstdlib>
#include <list>

struct TestHash
{
  size_t operator()(size_t i)
  {
    return i;
  }
};

#endif