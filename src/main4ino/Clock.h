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

#ifndef CLOCK_INC
#define CLOCK_INC

#include <main4ino/Misc.h>
#include <main4ino/Messages.h>
#include <main4ino/Configurable.h>
#include <main4ino/FreqConf.h>
#include <main4ino/Actor.h>
#include <log4ino/Log.h>

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600L
#define SECONDS_IN_MINUTE 60

#define SECS_CYCLE_FACTOR_DEFAULT 1.000f
#define SECS_CYCLE_FACTOR_INCR 0.0015f
#define SECS_CYCLE_FACTOR_DECR 0.0001f
#define SECS_CYCLE_FACTOR_MAX 1.400f
#define SECS_CYCLE_FACTOR_MIN 1.000f

#ifdef SUBCYCLES_1
#define SUB_CYCLES_PER_CYCLE 1
#endif // SUBCYCLES_1

#ifdef SUBCYCLES_2
#define SUB_CYCLES_PER_CYCLE 2
#endif // SUBCYCLES_2

#ifdef SUBCYCLES_4
#define SUB_CYCLES_PER_CYCLE 4
#endif // SUBCYCLES_4

#ifdef SUBCYCLES_8
#define SUB_CYCLES_PER_CYCLE 8
#endif // SUBCYCLES_8

enum ConfigSettings {
  ClockConfigStateHours = 0,
  ClockConfigStateMinutes,
  ClockConfigStateAdvanced,
  ClockConfigStateSeconds,
  ClockConfigStateFactorUp,
  ClockConfigStateFactorDown,
  ClockConfigStateShowSeconds,
  ClockConfigStateDelimiter
};

class Clock : public Configurable {

private:
  long t0;                 // amount of seconds from midnight as set up by the user
  long cyclesFromT0;       // amount of cycles passed from t0
  float secToCyclesFactor; // ratio seconds / cycle factor (normally bigger than 1)

  int nroActors;            // amount of actors
  Actor **actors;           // actors
  bool advancedConfig;      // flag indicating if advanced settings (like factor setup) will be available during configuration
  char nroInterruptsQueued; // amount of interrupts queuede at this moment
  bool showSeconds;         // flag telling if the seconds should be displayed in the LCD

  bool matches(int day, int hour, int minute);
  bool isValidMatch(FreqConf *fc);
  void invalidateFollowingMatches(FreqConf *fc);
  bool isFinalCycle();

public:
  Clock(Actor **a, int numberOfActors);

  bool matches(FreqConf *fc);
  void cycle();
  void increaseFactor();
  void decreaseFactor();
  float getFactor();
  void setFactor(float f);
  void set(int days, int hours, int minutes, int seconds);
  int getDays();
  int getHours();
  int getMinutes();
  int getSeconds();
  void increaseHour();
  void increaseMinute();
  void increaseSecond();
  void populateWithTime(char *buffer);
  long getSecondsFromT0();
  long getCyclesFromT0();
  void setNroInterruptsQueued(char n);

  const char *getName();
  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);
  int getNroInfos();
  void getInfo(int infoIndex, char *retroMsg);
};

#endif // CLOCK_INC
