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
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>
#include <string.h>

// Library being tested
#include <main4ino/Bot.h>
#include <main4ino/TestActor.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define WDT_INTERRUPT true

const char *EMPTY_MSG = "";

const char **lcdContentUp = &EMPTY_MSG;
const char **lcdContentDown = &EMPTY_MSG;

int indexConfigurable0 = 0; // to improve readability
int indexConfigurable1 = 1;
int indexConfigurable2 = 2;

void setUp(void) {
  lcdContentUp = &EMPTY_MSG;
  lcdContentDown = &EMPTY_MSG;
}

void tearDown(void) {}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("\nLCD:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentDown, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);

  TEST_ASSERT(strlen(str1) <= LCD_LENGTH);
  TEST_ASSERT(strlen(str2) <= LCD_LENGTH);
}

void test_bot_correctly_switches_modes(void) {
  int nroActors = 1;
  int nroConfigurables = nroActors + 1;

  int indexConfigurable1 = 1;
  TestActor a0("ACT0");

  Actor *dumbActors[] = {&a0, 0};
  Clock clock(dumbActors, nroActors);
  Configurable *configurables[] = {&a0, &clock, 0};

  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // nothing pressed

  // WELCOME MODE
  TEST_ASSERT_EQUAL(WelcomeMode, bot->getMode());
  TEST_ASSERT_EQUAL_STRING(MSG_BOT_STATE_WELCOME, *lcdContentUp);

  // HELP MODE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // nothing pressed
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

  // CONFIG CONFIGURABLES
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());              // first configurable (actor)
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot->getConfigurableStateIndex()); // first configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot->getConfigurableStateIndex()); // second configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode()); // second configurable (clock)
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateHours, bot->getConfigurableStateIndex());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // these are not supposed to affect at all mode changes
  bot->cycle(false, false, TimingInterruptSubCycle, 0.1f);
  bot->cycle(false, false, TimingInterruptSubCycle, 0.2f);

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateMinutes, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateAdvanced, bot->getConfigurableStateIndex());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // these are not supposed to affect at all mode changes
  bot->cycle(false, false, TimingInterruptSubCycle, 0.1f);
  bot->cycle(false, false, TimingInterruptSubCycle, 0.2f);

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode()); // done with actors

  // RUN STATE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(RunMode, bot->getMode());

  // CONFIG CONFIGURABLES STATE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());

  // and so on...
}

void test_bot_correctly_switches_modes_with_no_config_actor(void) {
  int nroActors = 1;
  int nroConfigurables = nroActors + 1;

  TestActor a0("ACT0");        // it has SOME configuration states
  TestActor a1("ACT1", false); // it has NO configuration states
  TestActor a2("ACT2");        // it has SOME configuration states

  Actor *dumbActors[] = {&a0, &a1, &a2, 0};
  Clock clock(dumbActors, nroActors);
  Configurable *configurables[] = {&a0, &a1, &a2, 0};

  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f); // WELCOME MODE -> HELP MODE

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f); // HELP MODE -> CONFIG CONFIGURABLES
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());              // first configurable (actor 0 with configurations)
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot->getConfigurableStateIndex()); // first configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());               // first configurable still
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot->getConfigurableStateIndex()); // second configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f); // jumps straight to actor 2 (as actor 1 does not have any configuration)
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable2, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot->getConfigurableStateIndex());
}

void test_bot_correctly_switches_infos(void) {
  int nroActors = 1;
  int nroConfigurables = 1;
  TestActor a0("ACTOR0");
  Actor *dumbActors[] = {&a0, 0}; // null terminated
  Clock clock(dumbActors, nroActors);
  Configurable *configurables[] = {&a0, 0}; // null terminated
  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->setMode(RunMode);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());        // first dumbActor info state (unique of the dumbActor itself)

  bot->cycle(false, SET_PRESSED, TimingInterruptNone, 0.0f);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(1, bot->getConfigurableStateIndex());        // second dumbActor info state (last watering time)

  bot->cycle(false, SET_PRESSED, TimingInterruptNone, 0.0f);

  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // back to the origin
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());
}

void test_bot_correctly_returns_full_status(void) {
  int nroActors = 1;
  int nroConfigurables = 1;
  char buffer[100];
  TestActor a0("ACTOR0", true);
  Actor *dumbActors[] = {&a0, 0}; // null terminated
  Clock clock(dumbActors, nroActors);
  Configurable *configurables[] = {&a0, 0}; // null terminated
  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->setMode(RunMode);

  buffer[0] = 0;
  bot->getConfigs(buffer);
  char* expectedConfigs = "ACTOR0.TA_CNF_1:0\nACTOR0.TA_CNF_2:0\n";
  TEST_ASSERT_EQUAL_STRING(expectedConfigs, buffer);

  buffer[0] = 0;
  bot->getInfos(buffer);
  char* expectedInfos = "ACTOR0.TA_INF_1:0\nACTOR0.TA_INF_2:0\n";
  TEST_ASSERT_EQUAL_STRING(expectedInfos, buffer);

  bot->setConfig(1, 5); // index 1 (ACTOR0.TA_INF_1), new value 5

  buffer[0] = 0;
  bot->getConfigs(buffer);
  char* expectedConfigs = "ACTOR0.TA_CNF_1:0\nACTOR0.TA_CNF_2:5\n";
  TEST_ASSERT_EQUAL_STRING(expectedConfigs, buffer);

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_modes);
  RUN_TEST(test_bot_correctly_switches_modes_with_no_config_actor);
  RUN_TEST(test_bot_correctly_switches_infos);
  RUN_TEST(test_bot_correctly_returns_full_status);
  UNITY_END();
}

#endif // BOT_TEST
#endif // UNIT_TEST
