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

#include <log4ino/Log.h>

#ifndef UNIT_TEST

char errorMsg[LCD_LENGTH + 1];

// !UNIT_TEST, SO ON-BOARD EXECUTION
#ifdef DEBUG

const char *logLevelStr[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

// Receive logs via serial port
char *getErrorLogged() {
  return errorMsg;
}

bool isThereErrorLogged() {
  return strlen(errorMsg) > 0;
}

void clearErrorLogged() {
  sprintf(errorMsg, "");
}

void setupLog() {
  Serial.begin(SERIAL_BAUDS);
  clearErrorLogged();
}

void log(const char *clz, LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
  if (l == Error) {
    sprintf(errorMsg, "%s", msg);
  }
}

void log(const char *clz, LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
  if (l == Error) {
    sprintf(errorMsg, "%d", msg);
  }
}

void log(const char *clz, LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.print(msg);
    Serial.print(" ");
    Serial.println(i);
    delay(DELAY_DEBUG_MS);
  }
  if (l == Error) {
    sprintf(errorMsg, "%s%d", msg, i);
  }
}

void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.print(msg1);
    Serial.print(" ");
    Serial.println(msg2);
    delay(DELAY_DEBUG_MS);
  }
  if (l == Error) {
    sprintf(errorMsg, "%s%s", msg1, msg2);
  }
}

#else // !DEBUG

char *getErrorLogged() {
  return errorMsg;
}

bool isThereErrorLogged() {
  return strlen(errorMsg) > 0;
}

void clearErrorLogged() {
  errorMsg[0] = 0;
}

// Do not generate logs
void setupLog() {
  clearErrorLogged();
}

void log(const char *clz, LogLevel l, const char *msg) {
  if (l == Error) {
    sprintf(errorMsg, "%s", msg);
  }
}

void log(const char *clz, LogLevel l, int msg) {
  if (l == Error) {
    sprintf(errorMsg, "%d", msg);
  }
}

void log(const char *clz, LogLevel l, const char *msg, int i) {
  if (l == Error) {
    sprintf(errorMsg, "%s%d", msg, i);
  }
}

void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {
  if (l == Error) {
    sprintf(errorMsg, "%s%s", msg1, msg2);
  }
}

#endif // DEBUG

#else // UNIT_TEST, SO ON-PC EXECUTION

const char *logLevelStr[4] = {KYEL "DEBUG" KNRM, KBLU "INFO " KNRM, KMAG "WARN " KNRM, KRED "ERROR" KNRM};

char *getErrorLogged() {
  return NULL;
}

bool isThereErrorLogged() {
  return false;
}

void clearErrorLogged() {}

void setupLog() {}

void log(const char *clz, LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%s]: %s\n", clz, logLevelStr[l], msg);
  }
}
void log(const char *clz, LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%s]: %d\n", clz, logLevelStr[l], msg);
  }
}

void log(const char *clz, LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%s]: %s %d\n", clz, logLevelStr[l], msg, i);
  }
}

void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%s]: %s %s\n", clz, logLevelStr[l], msg1, msg2);
  }
}

void delay(int ms) {}

#endif // UNIT_TEST
