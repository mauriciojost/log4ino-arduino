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
  setupLog(fcn, NULL);

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
  setupLog(fcn, "AA2;BB5;??3"); // AA from level 2, BB from level 5, the rest level 3

  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "AA"));
  TEST_ASSERT_EQUAL(false, hasToLog(Info, "AA"));
  TEST_ASSERT_EQUAL(false, hasToLog(Warn, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "AA"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "AA"));

  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "BB"));
  TEST_ASSERT_EQUAL(false, hasToLog(Info, "BB"));
  TEST_ASSERT_EQUAL(false, hasToLog(Warn, "BB"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "BB"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "BB"));

  TEST_ASSERT_EQUAL(false, hasToLog(Debug, "CC"));
  TEST_ASSERT_EQUAL(false, hasToLog(Info, "CC"));
  TEST_ASSERT_EQUAL(false, hasToLog(Warn, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(Error, "CC"));
  TEST_ASSERT_EQUAL(true, hasToLog(User, "CC"));

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_basic_behaviour);
  RUN_TEST(test_advanced_behaviour);
  return (UNITY_END());
}

#endif // UNIT_TEST
