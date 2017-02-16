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

#ifndef CONFIGURABLE_INC
#define CONFIGURABLE_INC

enum SetMode {
  SetNext,
  SetValue,
  DoNotSet
}

/**
* This class specifies the interface of configurables, components of the
* embedded system which:
* - can be configured via configuration states
* - can inform their state, via info states

These must be inherited by actors to be configured and inform their status.

*/
class Configurable {

public:
  /**
  * Retrieve the name of the configurable.
  */
  virtual const char *getName() = 0;

  /**
  * Get the number of configuration states of the configurable.
  */
  virtual int getNroConfigs() = 0;

  /**
  * Set a property given its index.
  * Calling this method fills in the retroMsg, useful to feed an LCD. Also the value parameter with the actual property value.
  *
  * - If set=SetNext, the property will change to the next value available
  * - If set=SetValue, the property will be set to the provided value
  * - If set=DoNotSet, the property will remain unchanged
  */
  virtual void setConfig(int configIndex, char *retroMsg, SetMode set, int* value) = 0;

  /**
  * Get the number of info states of the configurable.
  */
  virtual int getNroInfos() = 0;

  /**
  * Get an information status of the configurable.
  */
  virtual void getInfo(int infoIndex, char *retroMsg) = 0;
};

#endif // CONFIGURABLE_INC
