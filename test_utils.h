#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "extendible_hash.h"

#include <cstdlib>
#include <list>

//Simple hash functor that returns its input
struct TestHash
{
  size_t operator()(size_t i)
  {
    return i;
  }
};

#endif