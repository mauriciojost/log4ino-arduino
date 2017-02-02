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

#include <main4ino/Clock.h>

#define CLASS "Clock"
#define HOUR_MINUTE_SECOND_AMPM "%02d:%02d:%02d %s"
#define HOUR_MINUTE_AMPM "%02d:%02d %s"
#define INVALIDATE_PERIOD_SECONDS 119.0f

#define ONCE_H 24
#define ONCE_M 60

#define ANY 210
#define NEVER 211

Clock::Clock(Actor **a, int numberOfActors) {
  set(0, 0, 0, 0);
  actors = a;
  nroActors = numberOfActors;
  setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  advancedConfig = false;
  showSeconds = false;
}

bool Clock::matches(FreqConf *fc) {
  bool timeMatches = false;
  log(CLASS, Debug, "  CLK FREQ: ", fc->getFrequencyDescription());
  switch (fc->getFrequency()) {
    case OncePerMonth:
      timeMatches = matches(30, ONCE_H, ONCE_M);
      break;
    case TwicePerMonth:
      timeMatches = matches(15, ONCE_H, ONCE_M);
      break;
    case OncePerWeek:
      timeMatches = matches(07, ONCE_H, ONCE_M);
      break;
    case TwicePerWeek:
      timeMatches = matches(03, ONCE_H, ONCE_M);
      break;
    case ThreeTimesPerWeek:
      timeMatches = matches(02, ONCE_H, ONCE_M);
      break;
    case OncePerDay:
      timeMatches = matches(ANY, ONCE_H, ONCE_M);
      break;
    case TwicePerDay:
      timeMatches = matches(ANY, 12, ONCE_M);
      break;
    case OncePerHour:
      timeMatches = matches(ANY, 1, ONCE_M);
      break;
    case TwicePerHour:
      timeMatches = matches(ANY, ANY, 30);
      break;
    case OnceEvery5Minutes:
      timeMatches = matches(ANY, ANY, 5);
      break;
    case Never:
      timeMatches = matches(NEVER, NEVER, NEVER);
      break;
    default:
      timeMatches = false;
      break;
  }

  if (timeMatches) {
    if (isValidMatch(fc)) {
      log(CLASS, Info, "  CLK MATCH: ", fc->getFrequencyDescription());
      invalidateFollowingMatches(fc);
      return true;
    } else {
      log(CLASS, Debug, "  CLK (MUTE) FOR ", fc->getInvalidateCounter());
      return false;
    }
  } else {
    log(CLASS, Debug, "  CLK ZZZ");
    return false;
  }
}

void Clock::cycle() {
  cyclesFromT0++;
  if (isFinalCycle()) {
    set(0, 0, 0, 0);
  }
  for (int i = 0; i < nroActors; i++) {
    FreqConf *fc = actors[i]->getFrequencyConfiguration();
    fc->setInvalidateCounter(constrainValue(fc->getInvalidateCounter() - 1, 0, (int)(INVALIDATE_PERIOD_SECONDS / secToCyclesFactor)));
  }
  log(CLASS, Debug, "TICK ", (int)cyclesFromT0);
}

void Clock::set(int days, int hours, int minutes, int seconds) {
  t0 = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
  cyclesFromT0 = 0;
}

int Clock::getDays() {
  return getSecondsFromT0() / SECONDS_IN_DAY;
}

int Clock::getHours() {
  return (getSecondsFromT0() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
}

int Clock::getMinutes() {
  return (getSecondsFromT0() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
}

int Clock::getSeconds() {
  return getSecondsFromT0() % SECONDS_IN_MINUTE;
}

void Clock::increaseFactor() {
  secToCyclesFactor = rollValue(secToCyclesFactor + SECS_CYCLE_FACTOR_INCR, SECS_CYCLE_FACTOR_MIN, SECS_CYCLE_FACTOR_MAX);
}

void Clock::decreaseFactor() {
  secToCyclesFactor = rollValue(secToCyclesFactor - SECS_CYCLE_FACTOR_DECR, SECS_CYCLE_FACTOR_MIN, SECS_CYCLE_FACTOR_MAX);
}

float Clock::getFactor() {
  return secToCyclesFactor;
}

void Clock::setFactor(float f) {
  if (isnan(f)) {
    secToCyclesFactor = SECS_CYCLE_FACTOR_DEFAULT;
  } else {
    secToCyclesFactor = constrainValue(f, SECS_CYCLE_FACTOR_MIN, SECS_CYCLE_FACTOR_MAX);
  }
}

void Clock::increaseHour() {
  int h = getHours();
  int m = getMinutes();
  int nh = rollValue(h + 1, 0, 23);
  set(0, nh, m, 0);
}

void Clock::increaseMinute() {
  int h = getHours();
  int m = getMinutes();
  int nm = rollValue(m + 1, 0, 59);
  set(0, h, nm, 0);
}

void Clock::increaseSecond() {
  int h = getHours();
  int m = getMinutes();
  int s = getSeconds();
  int ns = rollValue(s + 1, 0, 59);
  set(0, h, m, ns);
}

void Clock::populateWithTime(char *buffer) {
  int h = getHours();
  int m = getMinutes();
  int s = getSeconds();
  bool am = h < 12;
  int nh = (h < 13 ? h : h - 12);
  if (am) {
    if (showSeconds) {
      sprintf(buffer, HOUR_MINUTE_SECOND_AMPM, nh, m, s, MSG_AM);
    } else {
      sprintf(buffer, HOUR_MINUTE_AMPM, nh, m, MSG_AM);
    }
  } else {
    if (showSeconds) {
      sprintf(buffer, HOUR_MINUTE_SECOND_AMPM, nh, m, s, MSG_PM);
    } else {
      sprintf(buffer, HOUR_MINUTE_AMPM, nh, m, MSG_PM);
    }
  }
}

bool Clock::matches(int day, int hour, int minute) {
  bool matchesDays = (((getDays() % day) == 0) || (day == ANY)) && (day != NEVER);
  bool matchesHours = (((getHours() % hour) == 0) || (hour == ANY)) && (hour != NEVER);
  bool matchesMinutes = (((getMinutes() % minute) == 0) || (minute == ANY)) && (minute != NEVER);
  bool allMatch = matchesDays && matchesHours && matchesMinutes;
  return allMatch;
}

bool Clock::isValidMatch(FreqConf *fc) {
  return fc->getInvalidateCounter() <= 0;
}

long Clock::getSecondsFromT0() {
  float secFromT0 = (cyclesFromT0 * secToCyclesFactor);
  return (long)round(secFromT0) + t0;
}

long Clock::getCyclesFromT0() {
  return (long)cyclesFromT0;
}

void Clock::invalidateFollowingMatches(FreqConf *fc) {
  fc->setInvalidateCounter((int)(INVALIDATE_PERIOD_SECONDS / secToCyclesFactor));
}

bool Clock::isFinalCycle() {
  return (getDays() == 30);
}

const char *Clock::getName() {
  return MSG_CLOCK_NAME;
}

int Clock::getNroConfigs() {
  if (advancedConfig) {
    return ClockConfigStateDelimiter;
  } else {
    return ClockConfigStateAdvanced + 1;
  }
}

void Clock::setConfig(int configIndex, char *retroMsg, bool set) {
  char timeBuffer[LCD_LENGTH + 1];
  switch (configIndex) {
    case (ClockConfigStateHours):
      if (set) {
        increaseHour();
      }
      populateWithTime(timeBuffer);
      sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_HOUR, timeBuffer);
      break;
    case (ClockConfigStateMinutes):
      if (set) {
        increaseMinute();
      }
      populateWithTime(timeBuffer);
      sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_MINUTE, timeBuffer);
      break;
    case (ClockConfigStateAdvanced):
      if (set) {
        advancedConfig = !advancedConfig;
      }
      sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_ADVANCED, (advancedConfig ? MSG_YES : MSG_NO));
      break;
    case (ClockConfigStateSeconds):
      if (set) {
        showSeconds = true;
        increaseSecond();
      }
      populateWithTime(timeBuffer);
      sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_SECOND, timeBuffer);
      break;
    case (ClockConfigStateFactorUp):
      if (set) {
        increaseFactor();
      }
      if (cyclesFromT0 % 2 == 0) {
        populateWithTime(timeBuffer);
        sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_FACTOR_UP, timeBuffer);
      } else {
        int d = (int)secToCyclesFactor;
        int f = (secToCyclesFactor - d) * 10000;
        sprintf(retroMsg, "%s%d.%04d", MSG_CLOCK_CONFIG_FACTOR_UP, d, f);
      }

      break;
    case (ClockConfigStateFactorDown):
      if (set) {
        decreaseFactor();
      }
      if (cyclesFromT0 % 2 == 0) {
        populateWithTime(timeBuffer);
        sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_FACTOR_DOWN, timeBuffer);
      } else {
        int d = (int)secToCyclesFactor;
        int f = (secToCyclesFactor - d) * 10000;
        sprintf(retroMsg, "%s%d.%04d", MSG_CLOCK_CONFIG_FACTOR_DOWN, d, f);
      }
      break;
    case (ClockConfigStateShowSeconds):
      if (set) {
        showSeconds = !showSeconds;
      }
      if (cyclesFromT0 % 2 == 0) {
        populateWithTime(timeBuffer);
        sprintf(retroMsg, "%s", timeBuffer);
      } else {
        sprintf(retroMsg, "%s%s", MSG_CLOCK_CONFIG_SHOW_SECONDS, (showSeconds ? MSG_YES : MSG_NO));
      }
      break;
    default:
      break;
  }
}

int Clock::getNroInfos() {
  return 2;
}

void Clock::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (0):
      populateWithTime(retroMsg);
      break;
    case (1):
      sprintf(retroMsg, "%s%d", MSG_CLOCK_INFO_NRO_INTS_QUEUED, (int)nroInterruptsQueued);
      break;
  }
}

void Clock::setNroInterruptsQueued(char n) {
  nroInterruptsQueued = n;
}
