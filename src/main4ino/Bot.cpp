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

#include <main4ino/Bot.h>
#include <main4ino/Messages.h>

#define CLASS "Bot"

#define DO_CHANGE true
#define DO_NOT_CHANGE false

BotModeData Bot::modesData[DelimiterAmountOfBotModes] = { // this must be aligned with the BotMode items
    {RunMode, &Bot::toRunMode, MSG_BOT_STATE_RUN, ConfigConfigurablesMode},
    {WelcomeMode, &Bot::toWelcomeMode, MSG_BOT_STATE_WELCOME, HelpMode},
    {HelpMode, &Bot::toHelpMode, MSG_BOT_STATE_HELP, ConfigConfigurablesMode},
    {ConfigConfigurablesMode, &Bot::toConfigConfigurablesMode, MSG_BOT_STATE_CONFIGURABLES, RunMode}};

Bot::Bot(Clock *clk, Actor **a, Configurable **c) {
  actors = a;
  configurables = c;
  clock = clk;
  mode = WelcomeMode;
  canChangeMode = true;
  configurableIndex = 0;
  configurableStateIndex = 0;

  int i;
  i = 0;
  while (actors[i] != NULL) {
    i++;
  }
  nroActors = i;
  i = 0;
  while (configurables[i] != NULL) {
    i++;
  }
  nroConfigurables = i;
  stdOutWriteStringFunction = NULL;
}

void Bot::setStdoutFunction(void (*wrSt)(const char *, const char *)) {
  stdOutWriteStringFunction = wrSt;
}

void Bot::cycle(bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle) {
  if (timingInterrupt == TimingInterruptCycle) {
    clock->cycle();
  }
  BotMode nextMode = mode; // no changes by default
  if (modePressed && canChangeMode) {
    nextMode = modesData[mode].nextMode;
    mode = nextMode;
    log(CLASS, Info, "->(NEXT) ST: ", modesData[mode].lcdMessage);
  } else {
    log(CLASS, Info, "->(SAME) ST: ", modesData[mode].lcdMessage);
  }
  (this->*modesData[nextMode].currentModeFunction)(&modesData[nextMode], modePressed, setPressed, timingInterrupt, subCycle);
}

void Bot::setMode(BotMode s) {
  mode = s;
}

int Bot::getMode() {
  return mode;
}

int Bot::getConfigurableIndex() {
  return configurableIndex;
}

int Bot::getConfigurableStateIndex() {
  return configurableStateIndex;
}

void Bot::toWelcomeMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle) {
  stdOutWriteString(data->lcdMessage, MSG_BOT_STATE_WELCOME_ZINO);
}

void Bot::toHelpMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle) {
  if (timingInterrupt == TimingInterruptCycle || modePressed || setPressed) {
    stdOutWriteString(MSG_BOT_STATE_HELP_UP, MSG_BOT_STATE_HELP_DOWN);
  }
}

void Bot::toRunMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle) {
  /* For this mode performance really matters.
   * Avoid manipulating strings unless they will be really displayed (button pressed or cycle).
   */
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (timingInterrupt != TimingInterruptNone) {
    for (int aIndex = 0; aIndex < nroActors; aIndex++) {
      if (timingInterrupt == TimingInterruptCycle) { // time matches are exclusive to cycles, not to subcycles
        bool match = clock->matches(actors[aIndex]->getFrequencyConfiguration());
        actors[aIndex]->cycle(match);
        actors[aIndex]->subCycle(subCycle);
      } else if (timingInterrupt == TimingInterruptSubCycle) {
        actors[aIndex]->subCycle(subCycle);
      }
    }
  } else if (setPressed) {
    nextInfoState();
  }
  if (timingInterrupt == TimingInterruptCycle || modePressed || setPressed) {
    updateInfo(lcdUp, lcdDown);
    stdOutWriteString(lcdUp, lcdDown);
  }
}

void Bot::toConfigConfigurablesMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle) {
  if (!modePressed && !setPressed && timingInterrupt == TimingInterruptSubCycle) { // Ignore these events
    return;
  }
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  bool change = DO_NOT_CHANGE;
  if (modePressed) {
    nextConfigurableConfigState();
  } else if (setPressed && !canChangeMode) { // set pressed and not done with configurables
    change = DO_CHANGE;
  }
  if (timingInterrupt == TimingInterruptCycle || modePressed || setPressed) {
    if (!canChangeMode) { // not yet done with configurables configuration
      sprintf(lcdUp, "%s %s", data->lcdMessage, configurables[configurableIndex]->getName());
      configurables[configurableIndex]->setConfig(configurableStateIndex, lcdDown, (change?SetNext:DoNotSet));
    } else { // done with actors configuration
      sprintf(lcdUp, "%s %s", data->lcdMessage, MSG_READY);
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_CONFIGURABLES);
    }
    stdOutWriteString(lcdUp, lcdDown);
  }
}

bool Bot::nextConfigurableConfigState(bool reset) {
  while (true) {
    if (canChangeMode || reset) { // just arrived to the config actors state or reset
      canChangeMode = false;
      configurableIndex = 0;
      configurableStateIndex = 0;
      int numConfigs = configurables[configurableIndex]->getNroConfigs();
      if (numConfigs > 0) {
        return true; // stop here, this configurable has some configurations
      }
    } else { // were here from previous cycle
      int numConfigs = configurables[configurableIndex]->getNroConfigs();
      configurableStateIndex++;

      if (configurableStateIndex >= numConfigs) { // no more configuration states for this configurable, we proceed until find a stop condition
        configurableIndex++;
        configurableStateIndex = 0;
        if (configurableIndex >= nroConfigurables) { // done with configurables configuration
          canChangeMode = true;
          configurableIndex = 0;
          configurableStateIndex = 0;
          return false;                                    // stop here, done with configurables configuration
        } else if (configurableIndex < nroConfigurables) { // still configurables to configure
          numConfigs = configurables[configurableIndex]->getNroConfigs();
          if (numConfigs > 0) {
            return true; // stop here, this configurable has some configurations
          }
        }
      } else if (configurableStateIndex < numConfigs) { // more configuration states for this configurable
        return true;                                    // stop here, this configurable has still some configurations left
      }
    }
  }
}

void Bot::updateInfo(char *lcdUp, char *lcdDown) {
  if (configurableIndex < nroConfigurables) { // infos for configurables
    int nroInfoStates = configurables[configurableIndex]->getNroInfos();
    sprintf(lcdUp, "%s %s", MSG_BOT_STATE_RUN, configurables[configurableIndex]->getName()); // LCDUP: RUN ACTOR0
    if (configurableStateIndex < nroInfoStates) {                                            // configurable infos
      configurables[configurableIndex]->getInfo(configurableStateIndex, lcdDown);
    }
  }
}

bool Bot::nextInfoState(bool reset) {
  while (true) {
    if (configurableIndex < nroConfigurables && !reset) { // infos for configurables
      int nroInfoStates = configurables[configurableIndex]->getNroInfos();
      configurableStateIndex++;
      if (configurableStateIndex >= nroInfoStates) { // number of info states for this configurable reached
        configurableIndex++;
        configurableStateIndex = -1;
      } else { // not reached so we stay here
        return true; // can continue
      }
    }
    if (configurableIndex >= nroConfigurables || reset) { // reset indexes
      configurableIndex = 0;
      configurableStateIndex = 0;
      return reset; // no more states
    }
  }
}

Clock *Bot::getClock() {
  return clock;
}

void Bot::stdOutWriteString(const char *up, const char *down) {
  if (stdOutWriteStringFunction != NULL) {
    stdOutWriteStringFunction(up, down);
  }
}
void Bot::getConfigs(char *body) {
  char buffer[LCD_LENGTH + 1];
  char number[LCD_LENGTH + 1];
  int counter = 0;
  while(nextConfigurableConfigState(counter == 0)) {
    sprintf(number, "%d.", configurableIndex);
    strcat(body, number);
    strcat(body, configurables[configurableIndex]->getName());
    strcat(body, ".");
    sprintf(number, "%d.", configurableStateIndex);
    strcat(body, number);
    configurables[configurableIndex]->setConfig(configurableStateIndex, buffer, DoNotSet);
    strcat(body, buffer);
    strcat(body, "\n");
    counter++;
  }
}

void Bot::getInfos(char *body) {
  char buffer[LCD_LENGTH + 1];
  char number[LCD_LENGTH + 1];
  int counter = 0;
  while(nextInfoState(counter == 0)) {
    sprintf(number, "%d.", configurableIndex);
    strcat(body, number);
    strcat(body, configurables[configurableIndex]->getName());
    strcat(body, ".");
    sprintf(number, "%d.", configurableStateIndex);
    strcat(body, number);
    configurables[configurableIndex]->getInfo(configurableStateIndex, buffer);
    strcat(body, buffer);
    strcat(body, "\n");
    counter++;
  }
}

