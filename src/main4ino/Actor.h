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

#ifndef ACTOR_INC
#define ACTOR_INC

#include <main4ino/Configurable.h>
#include <main4ino/FreqConf.h>

/**
* This class specifies the interface of actors, components of the
* embedded system which:
* - can be configured via configuration states (via [[Configurable]])
* - can inform their state, via info states (via [[Configurable]])
* - can inform the actuator value (if any) (a LED, a pump, a buzzer)
*/
class Actor : public Configurable {

public:
  /**
  * Make the actor act (heavy operations like LCD update, normally called every second).
  */
  virtual void cycle(bool cronMatches) = 0;

  /**
  * Make the actor act (light operations for smooth control over hardware, called very frequently).
  * Is passed with an (0.0f - 1.0f) float indicating the position within the current cycle.
  */
  virtual void subCycle(float subCycle) = 0;

  /**
  * Get the current actuator value.
  * It can be used to tell if a pump is on, the position of a servo, the level of a reservoir, etc.
  */
  virtual int getActuatorValue() = 0;

  /**
  * Get the frequency configuration for this actor.
  */
  virtual FreqConf *getFrequencyConfiguration() = 0;
};

#endif // ACTOR_INC
