//--- test/test/test_bp_test.hpp -------------------------------*- C++ -*-==//
//
//               Bellport Low Latency Trading Infrastructure.
//
// Copyright MayStreet 2012 - all rights reserved
//
// $Id$
//===--------------------------------------------------------------------===//
#ifndef BP_TEST_TEST_TEST_BP_TEST
#define BP_TEST_TEST_TEST_BP_TEST "$Id$"
#include "bp/test/bp_test.hpp"
#include <string>
#include <stdio.h>

namespace bp_test_test_test_bp_test {

TEST(BpTest, TestTestCreation) {
  printf("This is a test\n");
}

TEST(BpTest, TestExpect) {
  printf("This is a test\n");
  std::string val = "abc";
  EXPECT_EQ(std::string("abc"), val);
  EXPECT_EQ(std::string("abcd"), val);

  EXPECT_DOUBLE_EQ(1.0, 2.0);

  {
    SCOPED_TRACE("L1 scoped");
    {
      SCOPED_TRACE("Failed scope");
      EXPECT_EQ(std::string("zabc"), val);
      EXPECT_EQ(std::string("zabcd"), val);

      {
        SCOPED_TRACE("this is a success scoped trace");
      }
    }
  }
  {
    SCOPED_TRACE("L2 scoped");
  }
}


TEST(BpTest, TestAssert) {
  printf("This is a test\n");
  std::string val = "abc";
  ASSERT_EQ(std::string("abc"), val);
  ASSERT_EQ(std::string("abcd"), val);

  {
    SCOPED_TRACE("L1 scoped");
    {
      SCOPED_TRACE("Failed scope");
      ASSERT_EQ(std::string("zabc"), val);
      ASSERT_EQ(std::string("zabcd"), val);

      {
        SCOPED_TRACE("this is a success scoped trace");
      }
    }
  }
  {
    SCOPED_TRACE("L2 scoped");
  }
}

}

#endif  // BP_TEST_TEST_TEST_BP_TEST
