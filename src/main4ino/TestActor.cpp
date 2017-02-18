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
  config1 = 0;
  config2 = 0;
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

void TestActor::setConfig(int configIndex, char *retroMsg, SetMode set, int* value) {
  switch (configIndex) {
    case (TestActorConfigStateAmount):
      if (set == SetNext) {
        config1++;
      } else if (set == SetValue) {
        config1 = *value;
      }
      sprintf(retroMsg, "TA_CNF_1:%d", config1);
      if (value != NULL) { *value = config1;}
      break;
    case (TestActorConfigStateAmount2):
      if (set == SetNext) {
        config2++;
      } else if (set == SetValue) {
        config2 = *value;
      }
      sprintf(retroMsg, "TA_CNF_2:%d", config2);
      if (value != NULL) { *value = config2;}
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
      sprintf(retroMsg, "TA_INF_1:%d", config1);
      break;
    case (TestActorLastWatered):
      sprintf(retroMsg, "TA_INF_2:%d", config2);
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
