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

#include <main4ino/TestActor.h>

#define CLASS "TestActor"

TestActor::TestActor(const char *n, const bool enableConfigs) {
  name = n;
  on = false;
  configsEnabled = enableConfigs;
}

const char *TestActor::getName() {
  return name;
}

void TestActor::cycle(bool cronMatches) {
  if (cronMatches) {
    log(CLASS, Debug, "  TACT: ON");
    on = true;
  } else {
    log(CLASS, Debug, "  TACT: OFF");
    on = false;
  }
}

void TestActor::subCycle(float subCycle) {
  // not handled
}

int TestActor::getActuatorValue() {
  return on;
}

void TestActor::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
    case (TestActorConfigStateAmount):
      log(CLASS, Debug, "  TACT CNF: 1");
      sprintf(retroMsg, "TA_CNF_1");
      break;
    case (TestActorConfigStateAmount2):
      log(CLASS, Debug, "  TACT CNF: 1");
      sprintf(retroMsg, "TA_CNF_2");
      break;
    default:
      break;
  }
}

int TestActor::getNroConfigs() {
  if (configsEnabled) {
    return (int)TestActorConfigStateDelimiter;
  } else {
    return 0;
  }
}

void TestActor::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (TestActorInfoConfig):
      sprintf(retroMsg, "TA_INF_1");
      break;
    case (TestActorLastWatered):
      sprintf(retroMsg, "TA_INF_2");
      break;
    default:
      break;
  }
}

int TestActor::getNroInfos() {
  return (int)TestActorInfoDelimiter;
}

FreqConf *TestActor::getFrequencyConfiguration() {
  return &freqConf;
}

void TestActor::clearActuatorValue() {
  on = false;
}
