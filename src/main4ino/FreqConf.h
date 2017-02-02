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

#ifndef FREQCONF_INC
#define FREQCONF_INC

#include <main4ino/Messages.h>

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth,
  OncePerWeek,
  TwicePerWeek,
  ThreeTimesPerWeek,
  OncePerDay,
  TwicePerDay,
  OncePerHour,
  TwicePerHour,
  OnceEvery5Minutes,
  Never,
  DelimiterAmountOfFrequencies
};

class FreqConf {

private:
  Frequency freq;             // frequency associated to an actor
  int matchInvalidateCounter; // counter that allow to invalidate matches associated an actor

public:
  FreqConf(Frequency f = OncePerDay);

  void setFrequency(Frequency f);
  Frequency getFrequency();
  void setNextFrequency();
  const char *getFrequencyDescription();

  void setInvalidateCounter(int i);
  int getInvalidateCounter();

  bool isActivated();

};

#endif // FREQCONF_INC
