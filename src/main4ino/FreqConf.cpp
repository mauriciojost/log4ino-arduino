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

#include <main4ino/FreqConf.h>

const char *frequencyDescriptions[DelimiterAmountOfFrequencies] = {
  MSG_FREQ_1_PER_MONTH,
  MSG_FREQ_2_PER_MONTH,
  MSG_FREQ_1_PER_WEEK,
  MSG_FREQ_2_PER_WEEK,
  MSG_FREQ_3_PER_WEEK,
  MSG_FREQ_1_PER_DAY,
  MSG_FREQ_2_PER_DAY,
  MSG_FREQ_1_PER_HOUR,
  MSG_FREQ_2_PER_HOUR,
  MSG_FREQ_1_PER_5_MIN,
  MSG_FREQ_NEVER
};

FreqConf::FreqConf(Frequency f) {
  freq = f;
  matchInvalidateCounter = 0;
}

void FreqConf::setFrequency(Frequency f) {
  freq = f;
}

Frequency FreqConf::getFrequency() {
  return freq;
}

void FreqConf::setInvalidateCounter(int i) {
  matchInvalidateCounter = i;
}

int FreqConf::getInvalidateCounter() {
  return matchInvalidateCounter;
}

void FreqConf::setNextFrequency() {
  freq = (Frequency)((freq + 1) % DelimiterAmountOfFrequencies);
}

const char *FreqConf::getFrequencyDescription() {
  return frequencyDescriptions[freq];
}

bool FreqConf::isActivated() {
  return freq != Never;
}
