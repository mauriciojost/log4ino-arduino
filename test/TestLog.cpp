#ifdef UNIT_TEST

// Being tested
#include <Log.h>

// Extra libraries needed
#include <string.h>
#include <unity.h>

#define LOG_LEVEL 0

void setUp() {}

void tearDown() {}


void fcn(const char *msg, const char *clz, LogLevel l) {}

void test_basic_behaviour() {
  setupLog(fcn);

  TEST_ASSERT_EQUAL(NULL, getLogOptions());

  setLogLevel((char)Debug);
  TEST_ASSERT_EQUAL(true, hasToLog(Debug, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Info, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Warn, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "AA"));

  setLogLevel((char)Error);
  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "AA"));
  TEST_ASSERT_EQUAL(false, hasToLog(Info, "AA"));
  TEST_ASSERT_EQUAL(false, hasToLog(Warn, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "AA"));

}

void test_advanced_behaviour() {
  setupLog(fcn);
  setLogOptions("AA1;BB3;??1;");

  TEST_ASSERT_EQUAL_STRING("AA1;BB3;??1;", getLogOptions());

  // matching AA1;
  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Info, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Warn, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "AA"));

  // matching BB3;
  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "BB"));
  TEST_ASSERT_EQUAL(false, hasToLog(Info, "BB"));
  TEST_ASSERT_EQUAL(false, hasToLog(Warn, "BB"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "BB"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "BB"));

  // matching ??1;
  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(Info, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(Warn, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "CC"));

}

void test_log_options_set() {
  setupLog(fcn);
  setLogOptions("AA1;AA1;AA1;AA1;AA1;AA1;AA1;AA1;");
  TEST_ASSERT_EQUAL_STRING("AA1;AA1;AA1;AA1;AA1;AA1;AA1;AA1;", getLogOptions());
  setLogOptions("AA1;AA1;AA1;AA1;AA1;AA1;AA1;AA1;AA1;"); // one more, discarded
  TEST_ASSERT_EQUAL_STRING("AA1;AA1;AA1;AA1;AA1;AA1;AA1;AA1;", getLogOptions());
  setLogOptions(NULL);
  TEST_ASSERT_EQUAL(NULL, getLogOptions());
  setLogOptions("invalidlogopts"); // invalid
  TEST_ASSERT_EQUAL(NULL, getLogOptions());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_basic_behaviour);
  RUN_TEST(test_advanced_behaviour);
  RUN_TEST(test_log_options_set);
  return (UNITY_END());
}

#endif // UNIT_TEST
