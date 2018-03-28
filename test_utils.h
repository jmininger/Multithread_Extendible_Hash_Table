#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "extendible_hash.h"

#include <cstdlib>
#include <list>

struct TestHash
{
  size_t operator()(int i)
  {
    return i;
  }
};

#endif