//--- bp/test/bp_test.hpp --------------------------------------*- C++ -*-==//
//
//               Bellport Low Latency Trading Infrastructure.
//
// Copyright MayStreet 2012 - all rights reserved
//
// $Id$
//===--------------------------------------------------------------------===//
//
//===--------------------------------------------------------------------===//
#ifndef BP_TEST_BP_TEST
#define BP_TEST_BP_TEST "$Id$"

#ifdef WIN32
#include <windows.h>
#else
#endif

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#ifndef STR_VAL
#define STR_VAL2(X) #X
#define STR_VAL(X) STR_VAL2(X)
#endif

#if _MSC_VER
#define LONG_LONG __int64
#else
#define LONG_LONG long long
// #define LONG_LONG int64_t
#endif /* _MSC_VER */

#ifdef _MSC_VER
  typedef signed char         int8_t;
  typedef unsigned char       uint8_t;
  typedef short               int16_t;
  typedef unsigned short      uint16_t;
  typedef int                 int32_t;
  typedef unsigned int        uint32_t;
  typedef long long           int64_t;
  typedef unsigned long long  uint64_t;
  typedef float               real32_t;
  typedef double              real64_t;
#else
#  include <stdint.h>
  typedef float               real32_t;
  typedef double              real64_t;
#endif

namespace bp_detail {
namespace detail {
  template <typename SOURCE, typename DEST>
  union UnionCastUnion {
    SOURCE source;
    DEST   dest;
  };
}

template <typename DEST, typename SOURCE>
DEST UnionCast(SOURCE source) {
  detail::UnionCastUnion<SOURCE, DEST> union_cast_union;
  union_cast_union.source = source;
  return union_cast_union.dest;
}

bool DoubleCompare(double d1, double d2) {
  uint64_t i1 = UnionCast<uint64_t>(d1);
  uint64_t i2 = UnionCast<uint64_t>(d2);
  return (i1 & 0xFFFFFFFFFFFFFFFC) == (i2 & 0xFFFFFFFFFFFFFFFC);
}

bool FloatCompare(float f1, float f2) {
  uint32_t i1 = UnionCast<uint32_t>(f1);
  uint32_t i2 = UnionCast<uint32_t>(f2);
  return (i1 & 0xFFFFFFFC) == (i2 & 0xFFFFFFFC);
}
}
namespace detail_stream {
  typedef char yes;
  typedef char (&no)[2];

  struct anyx { template <class T> anyx(const T &); };

  no operator << (const anyx &, const anyx &);
  no operator >> (const anyx &, const anyx &);

  template <class T> yes check(T const&);
  no check(no);
  
  template <typename StreamType, typename T>
  struct SavingStreamOpExists {
    static StreamType& stream;
    static T& x;
    static const bool value = sizeof(check(stream << x)) == sizeof(yes);
  };

  template <typename StreamType, int>
  struct OStreamer {
    template <typename T>
    static void Stream(StreamType& stream, const T& val) { stream << val; }
  };

  template <typename StreamType>
  struct OStreamer<StreamType, 0> {
    template <typename T>
    static void Stream(StreamType& stream, const T& val) { stream << &val; }
  };

  template <typename T>
  struct StreamObj {
    StreamObj(const T& theval)
      : val(theval)
    {}
    const T& val;
  };

  template <typename StreamType, typename T>
  StreamType& operator<< (StreamType& stream, const StreamObj<T>& rhs) {
    OStreamer<StreamType, SavingStreamOpExists<StreamType, T>::value >::Stream(stream, rhs.val);
    return stream;
  }

  template <typename T>
  StreamObj<T> Stream(const T& val) {
    return StreamObj<T>(val);
  }
}

#define ASSERT_TRUE(EXPECTED)              { if(!(EXPECTED))                      { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_TRUE("      #EXPECTED                ") "                                                                                  " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_FALSE(EXPECTED)             { if((EXPECTED))                       { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_FALSE("     #EXPECTED                ") "                                                                                  " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_LT(EXPECTED, ACTUAL)        { if(!(EXPECTED <  ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_LT("        #EXPECTED " < "  #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " < "  << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_LE(EXPECTED, ACTUAL)        { if(!(EXPECTED <= ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_LE("        #EXPECTED " <= " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " <= " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_GT(EXPECTED, ACTUAL)        { if(!(EXPECTED >  ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_GT("        #EXPECTED " > "  #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " > "  << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_GE(EXPECTED, ACTUAL)        { if(!(EXPECTED >= ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_GE("        #EXPECTED " >= " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " >= " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_EQ(EXPECTED, ACTUAL)        { if(!(EXPECTED == ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_EQ("        #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_NE(EXPECTED, ACTUAL)        { if(!(EXPECTED != ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_NE("        #EXPECTED " != " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " != " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_STREQ(EXPECTED, ACTUAL)     { if(0 != strcmp(EXPECTED, ACTUAL))    { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_STREQ("     #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_STRNE(EXPECTED, ACTUAL)     { if(0 == strcmp(EXPECTED, ACTUAL))    { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_STRNE("     #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define ASSERT_DOUBLE_EQ(EXPECTED, ACTUAL) { if(!DoubleCompare(EXPECTED, ACTUAL)) { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error ASSERT_DOUBLE_EQ(" #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; return;} }
#define EXPECT_TRUE(EXPECTED)              { if(!(EXPECTED))                      { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_TRUE("      #EXPECTED                ") "                                                                                  " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_FALSE(EXPECTED)             { if((EXPECTED))                       { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_FALSE("     #EXPECTED                ") "                                                                                  " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_LT(EXPECTED, ACTUAL)        { if(!(EXPECTED <  ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_LT("        #EXPECTED " < "  #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " < "  << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_LE(EXPECTED, ACTUAL)        { if(!(EXPECTED <= ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_LE("        #EXPECTED " <= " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " <= " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_GT(EXPECTED, ACTUAL)        { if(!(EXPECTED >  ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_GT("        #EXPECTED " > "  #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " > "  << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_GE(EXPECTED, ACTUAL)        { if(!(EXPECTED >= ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_GE("        #EXPECTED " >= " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " >= " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_EQ(EXPECTED, ACTUAL)        { if(!(EXPECTED == ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_EQ("        #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_NE(EXPECTED, ACTUAL)        { if(!(EXPECTED != ACTUAL))            { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_NE("        #EXPECTED " != " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " != " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_STREQ(EXPECTED, ACTUAL)     { if(0 != strcmp(EXPECTED, ACTUAL))    { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_STREQ("     #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_STRNE(EXPECTED, ACTUAL)     { if(0 == strcmp(EXPECTED, ACTUAL))    { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_STRNE("     #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_DOUBLE_EQ(EXPECTED, ACTUAL) { if(!bp_detail::DoubleCompare(EXPECTED, ACTUAL)) { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_DOUBLE_EQ(" #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }
#define EXPECT_FLOAT_EQ(EXPECTED, ACTUAL)  { if(!bp_detail::FloatCompare(EXPECTED, ACTUAL))  { BP_TEST_LIGHT_BLUE; std::stringstream ss; ss << __FILE__ "(" STR_VAL(__LINE__) "): error EXPECT_DOUBLE_EQ(" #EXPECTED " == " #ACTUAL ") " << detail_stream::Stream(EXPECTED) << " == " << detail_stream::Stream(ACTUAL) << " failed"; BP_TEST_REPORT_MESSAGE_1(ss.str().c_str()); ++(bp::test::TestCaseManager::Instance()->error_count_); BP_TEST_WHITE; } }

#define SCOPED_TRACE(X) bp::test::ScopedTrace scoped_trace(X);

#define BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME) TEST_CASE_NAME##_##TEST_NAME

#define BP_TEST(TEST_CASE_NAME, TEST_NAME) BP_TEST_INTERNAL(TEST_CASE_NAME, TEST_NAME)
// Define this macro to 1 to omit the definition of TEST(), which
// is a generic name and clashes with some other libraries.1
#if !BP_TEST_DONT_DEFINE_TEST
#define TEST(TEST_CASE_NAME, TEST_NAME) BP_TEST(TEST_CASE_NAME, TEST_NAME)
#endif

#define BP_TEST_RUN_ALL_TESTS() bp::test::TestCaseManager::Instance()->RunAllTests()

#if !BP_TEST_DONT_DEFINE_RUN_ALL_TESTS
#define RUN_ALL_TESTS() BP_TEST_RUN_ALL_TESTS()
#endif


////////////////////////////////////////////
////////////////////////////////////////////
// Internal
////////////////////////////////////////////
////////////////////////////////////////////

#ifndef BP_TEST_INDENT
#define BP_TEST_INDENT "  "
#endif

#ifdef WIN32
#define BP_TEST_RED SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x0C );
#define BP_TEST_MAGENTA SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x0D );
#define BP_TEST_YELLOW SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x0E );
#define BP_TEST_WHITE SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x07 );
#define BP_TEST_GREEN SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x0A );
#define BP_TEST_LIGHT_BLUE SetConsoleTextAttribute( GetStdHandle( STD_ERROR_HANDLE ), 0x00B );
#else
// http://stackoverflow.com/questions/138383/colored-grep/138528#138528
#define BP_TEST_RED     std::cerr << "\033[1m\033[31m";      /* Red */
#define BP_TEST_GREEN   std::cerr << "\033[32m";      /* Green */
#define BP_TEST_YELLOW  std::cerr << "\033[1m\033[33m";      /* Yellow */
#define BP_TEST_MAGENTA std::cerr << "\033[1m\033[35m";      /* Magenta */
#define BP_TEST_WHITE   std::cerr << "\033[0m";      /* Back to Normal */
#define BP_TEST_LIGHT_BLUE   std::cerr << "\033[01;34m";      /* Back to Normal */

/* #define BP_TEST_RED */
/* #define BP_TEST_MAGENTA */
/* #define BP_TEST_YELLOW */
/* #define BP_TEST_WHITE */
/* #define BP_TEST_GREEN */
#endif

// support printing to the output window if the user is on windows so they can click to go to the error line
#ifdef WIN32
#define BP_TEST_REPORT_MESSAGE_1(X) { \
  for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { OutputDebugString(BP_TEST_INDENT); }\
  OutputDebugString(X);\
  OutputDebugString("\r\n"); \
  for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { std::cerr << BP_TEST_INDENT; } \
  std::cerr << X << std::endl; }
#else
#define BP_TEST_REPORT_MESSAGE_1(X) { for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { std::cerr << BP_TEST_INDENT; } std::cerr << X << std::endl; }
#endif

#ifdef WIN32
#define BP_TEST_REPORT_MESSAGE_2(X1, X2) { \
  for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { OutputDebugString(BP_TEST_INDENT); }\
  OutputDebugString(X1);\
  OutputDebugString(X2); \
  OutputDebugString("\r\n"); \
  for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { std::cerr << BP_TEST_INDENT; } \
  std::cerr << X1 << X2 << std::endl;\
 }
#else
#define BP_TEST_REPORT_MESSAGE_2(X1, X2) { \
  for(size_t i = 0; i < bp::test::TestCaseManager::Instance()->scope_stack_.size(); ++i) { std::cerr << BP_TEST_INDENT; } \
  std::cerr << X1 << X2 << std::endl;\
 }
#endif

#define BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME) TEST_CASE_NAME##_##TEST_NAME
#define BP_TEST_INTERNAL(TEST_CASE_NAME, TEST_NAME) \
class BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME) : public ::bp::test::ITest {\
public: \
  virtual void TestBody();\
  virtual const char* GetTestCaseName() { return #TEST_CASE_NAME;};\
  virtual const char* GetTestName() { return #TEST_NAME;};\
  static size_t tmp;\
  };\
  size_t BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME)::tmp = ::bp::test::TestCaseManager::Instance()->RegisterTest(new BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME)()); \
  void BP_TEST_CLASS_NAME(TEST_CASE_NAME, TEST_NAME)::TestBody()

namespace bp {
namespace test {

class ITest {
public:
  virtual ~ITest() {}
  virtual void TestBody() = 0;
  virtual const char* GetTestCaseName() = 0;
  virtual const char* GetTestName() = 0;
};

class TestCaseManager {
public:

  TestCaseManager()
    : all_tests_(),
      scope_stack_(),
      error_count_(0),
      errors_()
  {}

  ~TestCaseManager() {
    Shutdown();
  }

  static TestCaseManager* Instance() {
    static TestCaseManager manager;
    return &manager;
  }
  size_t RegisterTest(ITest* test) {
    all_tests_.push_back(test);
    return all_tests_.size();
  }
  inline int RunAllTests();

  std::pair<std::string, int> Pop() {
    std::pair<std::string, int> ret_val = scope_stack_.at(scope_stack_.size()-1);
    scope_stack_.pop_back();
    return ret_val;
  }
  void Push(const std::string& msg) {
    scope_stack_.push_back(std::make_pair(msg, error_count_));
  }

  std::string GetStackString() {
    std::string res = "";
    for (size_t i = 0; i < scope_stack_.size(); ++i) {
      res += scope_stack_[i].first;
      if (i != (scope_stack_.size() - 1)) {
        res += " -> ";
      }
    }
    return res;
  }

  void Shutdown() {
    std::stringstream ss;
    ss << "| " << (all_tests_.size() - errors_.size()) << " of " << all_tests_.size() << " Tests Succeededs";

    BP_TEST_GREEN;
    BP_TEST_REPORT_MESSAGE_1("| ------- |");
    BP_TEST_REPORT_MESSAGE_1("| SUCCESS |");
    BP_TEST_REPORT_MESSAGE_1(ss.str().c_str());
    BP_TEST_REPORT_MESSAGE_1("| ------- |");
    BP_TEST_WHITE;
    if (errors_.size() > 0) {
      BP_TEST_RED;
      BP_TEST_REPORT_MESSAGE_1("| SUMMARY |");
      BP_TEST_REPORT_MESSAGE_1("| ------- |");
      BP_TEST_WHITE;
    }
    for(size_t global_err_num = 0; global_err_num < errors_.size(); ++global_err_num) {
      BP_TEST_RED;
      BP_TEST_REPORT_MESSAGE_2("| FAILURE | ",  errors_[global_err_num].c_str());
      BP_TEST_WHITE;
    }
    if (errors_.size() > 0) {
      BP_TEST_RED;
      BP_TEST_REPORT_MESSAGE_1("| ------- |");
      BP_TEST_WHITE;
    }

    all_tests_.clear();
    scope_stack_.clear();
    error_count_ = 0;
    errors_.clear();
  }

  std::vector<ITest*>                       all_tests_;
  std::vector<std::pair<std::string, int> > scope_stack_;
  int                                       error_count_;
  std::vector<std::string>                  errors_;
};


struct ScopedTrace {
  ScopedTrace(const std::string& msg)
  : msg_(msg)
  {
    TestCaseManager::Instance()->Push(msg);
    std::string str = TestCaseManager::Instance()->GetStackString();
    TestCaseManager::Instance()->Pop();
    BP_TEST_GREEN;
    BP_TEST_REPORT_MESSAGE_2("| BEGIN   | ", str.c_str());
    BP_TEST_WHITE;
    TestCaseManager::Instance()->Push(msg);
  }
  ~ScopedTrace() {
    std::string stack_string = TestCaseManager::Instance()->GetStackString();
    if (TestCaseManager::Instance()->Pop().second == TestCaseManager::Instance()->error_count_) {
      BP_TEST_GREEN;
      BP_TEST_REPORT_MESSAGE_2("| SUCCESS | ", stack_string.c_str());
      BP_TEST_WHITE;
    } else {;
      BP_TEST_RED;
      BP_TEST_REPORT_MESSAGE_2("| FAILURE | ", stack_string.c_str());
      TestCaseManager::Instance()->errors_.push_back(stack_string);
      BP_TEST_WHITE;
    }
  }
  std::string msg_;
};

inline int TestCaseManager::RunAllTests() {
  for(std::vector<ITest*>::iterator cur = all_tests_.begin(), end = all_tests_.end(); cur != end; ++cur) {
    bp::test::ScopedTrace scoped_test_reporter(std::string((*cur)->GetTestCaseName()) + std::string((*cur)->GetTestName()));
    (*cur)->TestBody();
  }
  return errors_.size();
}

}  // namespace test
}  // namespace bp














#endif  // BP_TEST_BP_TEST
