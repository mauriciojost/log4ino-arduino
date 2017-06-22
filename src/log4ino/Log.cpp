/*
 * LOG4INO
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of LOG4INO.
 *
 * LOG4INO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * LOG4INO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LOG4INO. If not, see <http://www.gnu.org/licenses/>.
 */

#include <log4ino/Log.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef UNIT_TEST

// !UNIT_TEST, SO ON-BOARD EXECUTION
#ifdef DEBUG

const char *logLevelStr[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

void setupLog() {
  Serial.begin(SERIAL_BAUDS);
}

bool readAvailable() {
  return Serial.available();
}

int readByte() {
  return Serial.read();
}

void log(const char *clz, LogLevel l, const char *format, ...) {
  if (LOG_LEVEL <= l) {
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 100, format, args);
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(buffer);
    delay(DELAY_DEBUG_MS);
    va_end(args);
  }
}

#else // !DEBUG

// Do not generate logs
void setupLog() { }

bool readAvailable() { return false; }

int readByte() { return 0; }

void log(const char *clz, LogLevel l, const char *format, ...) { }

#endif // DEBUG

#else // UNIT_TEST, SO ON-PC EXECUTION

// ON-PC SHELL COLORS (to be used with printf in PC tests)
// http://ascii-table.com/ansi-escape-sequences.php
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KMAG "\x1B[35m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KWHTBLU "\x1B[37;44m"
const char *logLevelStr[4] = {KYEL "DEBUG" KNRM, KBLU "INFO " KNRM, KMAG "WARN " KNRM, KRED "ERROR" KNRM};

void setupLog() {}

void log(const char *clz, LogLevel l, const char *format, ...) {
  if (LOG_LEVEL <= l) {
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 100, format, args);
    printf("[%8.8s] [%s]: %s %s\n", clz, logLevelStr[l], buffer);
    va_end(args);
  }
}

void delay(int ms) {}

#endif // UNIT_TEST
