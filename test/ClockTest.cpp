/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef UNIT_TEST
#ifdef CLOCK_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <main4ino/Clock.h>
#include <main4ino/TestActor.h>

void setUp(void) {}

void tearDown(void) {}

bool isFinalCycle(Clock *clock) {
  return
    (clock->getDays() == 29) &&
    (clock->getHours() == 23) &&
    (clock->getMinutes() == 59) &&
    (clock->getSeconds() == 59);
}

void test_clock_advances_time(void) {

  long c = 0;
  int nroActors = 1;

  TestActor a0("ACT0");
  Actor *dumbActors[] = {&a0, 0};
  Clock clock(dumbActors, nroActors);

  while (!isFinalCycle(&clock)) {
    TEST_ASSERT_EQUAL(c, clock.getCyclesFromT0());
    float expectedSeconds = c * SECS_CYCLE_FACTOR_DEFAULT;
    TEST_ASSERT_FLOAT_WITHIN(0.5f, expectedSeconds, clock.getSecondsFromT0());
    clock.cycle();
    c++;
  }

  int offDay = 0;
  int offHou = 23;
  int offMin = 1;
  int offSec = 2;

  clock.set(offDay, offHou, offMin, offSec);
  long t0 = offDay * 3600 * 24 + offHou * 3600 + offMin * 60 + offSec;

  c = 0;
  while (!isFinalCycle(&clock)) {
    float expectedSecondsFromT0 = (c * SECS_CYCLE_FACTOR_DEFAULT) + t0;

    TEST_ASSERT_EQUAL(c, clock.getCyclesFromT0());
    TEST_ASSERT_FLOAT_WITHIN(0.6f, expectedSecondsFromT0, clock.getSecondsFromT0());

    clock.cycle();
    c++;
  }

  TEST_ASSERT_EQUAL(29, clock.getDays());
  TEST_ASSERT_EQUAL(23, clock.getHours());
  TEST_ASSERT_EQUAL(59, clock.getMinutes());
  TEST_ASSERT_EQUAL(59, clock.getSeconds());

  clock.cycle();

  TEST_ASSERT_EQUAL(0, clock.getDays());
  TEST_ASSERT_EQUAL(0, clock.getHours());
  TEST_ASSERT_EQUAL(0, clock.getMinutes());
  TEST_ASSERT_EQUAL(0, clock.getSeconds());
}

void test_clock_correctly_sets_time(void) {
  int nroActors = 1;
  TestActor a0("ACT0");
  Actor *dumbActors[] = {&a0, 0};
  Clock clock(dumbActors, nroActors);
  for (int d = 0; d < 31; d++) {
    for (int h = 0; h < 24; h++) {
      for (int m = 0; m < 60; m++) {
        for (int s = 0; s < 60; s++) {
          clock.set(d, h, m, s);
          TEST_ASSERT_EQUAL(d, clock.getDays());
          TEST_ASSERT_EQUAL(h, clock.getHours());
          TEST_ASSERT_EQUAL(m, clock.getMinutes());
          TEST_ASSERT_EQUAL(s, clock.getSeconds());
        }
      }
    }
  }
}

int count_waterings_in_30days(Frequency f) {
  int count = 0;
  int nroActors = 1;
  TestActor a0("ACT0");
  Actor *dumbActors[] = {&a0, 0};
  Clock clock(dumbActors, nroActors);
  FreqConf *fc = a0.getFrequencyConfiguration();
  fc->setFrequency(f);
  while (!isFinalCycle(&clock)) {
    clock.cycle();
    if (clock.matches(fc)) {
      count++;
    }
  }
  return count;
}

void test_clock_correctly_tells_time_to_water(void) {
  TEST_ASSERT_EQUAL(0, count_waterings_in_30days(Never));
  TEST_ASSERT_EQUAL(1, count_waterings_in_30days(OncePerMonth));
  TEST_ASSERT_EQUAL(2, count_waterings_in_30days(TwicePerMonth));
  TEST_ASSERT_EQUAL(5, count_waterings_in_30days(OncePerWeek));
  TEST_ASSERT_EQUAL(10, count_waterings_in_30days(TwicePerWeek));
  TEST_ASSERT_EQUAL(15, count_waterings_in_30days(ThreeTimesPerWeek));
  TEST_ASSERT_EQUAL(30, count_waterings_in_30days(OncePerDay));
  TEST_ASSERT_EQUAL(60, count_waterings_in_30days(TwicePerDay));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_clock_advances_time);
  RUN_TEST(test_clock_correctly_sets_time);
  RUN_TEST(test_clock_correctly_tells_time_to_water);
  UNITY_END();
}

#endif // CLOCK_TEST
#endif // UNIT_TEST
