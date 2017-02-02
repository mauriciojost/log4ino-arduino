#ifdef UNIT_TEST
#ifdef MISC_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <main4ino/Misc.h>

void constrain_value_test(void) {

  int a = 0;
  int inc = 1;
  int min = 0;
  int max = 2;

  a = constrainValue(a - inc, min, max);
  TEST_ASSERT_EQUAL(0, a);

  a = constrainValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(1, a);

  a = constrainValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(2, a);

  a = constrainValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(2, a);
}

void roll_value_test(void) {

  int a = 0;
  int inc = 1;
  int min = 0;
  int max = 2;

  a = rollValue(a - inc, min, max);
  TEST_ASSERT_EQUAL(2, a);

  a = rollValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(0, a);

  a = rollValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(1, a);

  a = rollValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(2, a);

  a = rollValue(a + inc, min, max);
  TEST_ASSERT_EQUAL(0, a);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(constrain_value_test);
  RUN_TEST(roll_value_test);
  UNITY_END();
}

#endif
#endif
