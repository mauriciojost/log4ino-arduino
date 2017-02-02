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

#ifndef MESSAGES_INC
#define MESSAGES_INC

// GENERAL

#define MSG_AM "am"
#define MSG_PM "pm"

#define MSG_YES "YES"
#define MSG_NO "NO"

#define MSG_ON "\2ON"
#define MSG_OFF "\2OFF"

#define MSG_READY "READY"

#define MSG_ERROR "ERROR"

#define MSG_FREQ "\2Freq? "

// BOT

#define MSG_BOT_STATE_WELCOME "\1INTRO\1"
#define MSG_BOT_STATE_WELCOME_ZINO "ZINO " STRINGIFY(VERSION)

#define MSG_BOT_STATE_HELP "HELP"
#define MSG_BOT_STATE_HELP_UP "\1 CHANGES MODE"
#define MSG_BOT_STATE_HELP_DOWN "\2 SETS VALUE"

#define MSG_BOT_STATE_RUN "\3RUN\3 "

#define MSG_BOT_STATE_CONFIGURABLES "\1CONF"
#define MSG_BOT_DONE_CONFIGURING_CONFIGURABLES "PRESS\1"

// PUMP

#define MSG_PUMP_CONFIG_AMOUNT "\2Amount? "
#define MSG_PUMP_CONFIG_VALUE_RANGE "\2Range? "
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST "\2Test? "
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST_YES "\2ON RUN"
#define MSG_PUMP_INFO_LAST_WATERING "Last: "
#define MSG_PUMP_INFO_WATERING_COUNT "Count: "

#define MSG_PUMP_NAME0 "PLANT_A"
#define MSG_PUMP_NAME1 "PLANT_B"
#define MSG_PUMP_NAME2 "PLANT_C"
#define MSG_PUMP_NAME3 "PLANT_D"

// LEVEL

#define MSG_LEVEL_CONFIG_MINIMUM "\2Min: "
#define MSG_LEVEL_CONFIG_MAXIMUM "\2Max: "
#define MSG_LEVEL_INFO_CURRENT_LEVEL "Lev: "
#define MSG_LEVEL_NAME "LEVEL"
#define MSG_LEVEL_CONFIG_ADVANCED "\2Advanced? "

// CLOCK

#define MSG_CLOCK_NAME "TIME"
#define MSG_CLOCK_CONFIG_HOUR "\2H? "
#define MSG_CLOCK_CONFIG_MINUTE "\2M? "
#define MSG_CLOCK_CONFIG_ADVANCED "\2Advanced? "
#define MSG_CLOCK_CONFIG_SECOND "\2S? "
#define MSG_CLOCK_CONFIG_FACTOR_UP "\2F+? "
#define MSG_CLOCK_CONFIG_FACTOR_DOWN "\2F-? "
#define MSG_CLOCK_CONFIG_SHOW_SECONDS "Show secs? "
#define MSG_CLOCK_INFO_NRO_INTS_QUEUED "QUEUE "

// FREQUENCIES

#define MSG_FREQ_1_PER_MONTH "1/month"
#define MSG_FREQ_2_PER_MONTH "2/month"
#define MSG_FREQ_1_PER_WEEK "1/week"
#define MSG_FREQ_2_PER_WEEK "2/week"
#define MSG_FREQ_3_PER_WEEK "3/week"
#define MSG_FREQ_1_PER_DAY "1/day"
#define MSG_FREQ_2_PER_DAY "2/day"
#define MSG_FREQ_1_PER_HOUR "1/hour"
#define MSG_FREQ_2_PER_HOUR "2/hour"
#define MSG_FREQ_1_PER_5_MIN "1/5min"
#define MSG_FREQ_NEVER "NEVER"

#endif // MESSAGES_INC
