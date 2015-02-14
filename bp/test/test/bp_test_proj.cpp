//--- test/test/bp_test_proj.cpp -------------------------------*- C++ -*-==//
//
//               Bellport Low Latency Trading Infrastructure.
//
// Copyright MayStreet 2012 - all rights reserved
//
// $Id$
//===--------------------------------------------------------------------===//
#define CPP_SHA1_ID_STRING "$Id$"

#include "bp/test/test/test_bp_test.hpp"

#include <stdio.h>

int main(int argc, char* argv[])
{
  std::cerr << "__cplusplus " << __cplusplus << std::endl;

  std::cerr << "Running main() from " __FILE__ " " CPP_SHA1_ID_STRING "\n";

  int res = RUN_ALL_TESTS();

  return res;
}

