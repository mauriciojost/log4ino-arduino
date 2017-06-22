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

#ifndef LOG_INC
#define LOG_INC

#define DELAY_DEBUG_MS 5
#define SERIAL_BAUDS 115200

enum LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3 };

void setupLog();
bool readAvailable();
int readByte();
void log(const char *clz, LogLevel l, const char *format, ...);

#ifndef UNIT_TEST

#include <Arduino.h>

#else // UNIT_TEST, SO ON-PC EXECUTION

#include <stdio.h>
void delay(int ms);

#endif // UNIT_TEST

#endif // LOG_INC
