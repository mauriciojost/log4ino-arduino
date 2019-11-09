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

// Debug
// Info
// Warning
// Error
// User (highest level, used for interaction with the user)
enum LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3, User = 4};

void setupLog(void (*prnt)(const char *msg, const char *clz, LogLevel l));
void setLogLevel(char level);
char getLogLevel();

// Log with a given class identifier and a log level, the formatted arguments
void log(const char *clz, LogLevel l, const char *format, ...);

// Log with a given class identifier and a log level, in hex representation the given buffer
void logHex(const char *clz, LogLevel l, const unsigned char *buf, int bytes);

// Log with a given class identifier and a log level, the string as is
void logRaw(const char *clz, LogLevel l, const char *raw);

#ifdef ARDUINO

#include <Arduino.h>

#endif // ARDUINO

#ifdef X86_64

#include <stdio.h>

#endif // X86_64

#endif // LOG_INC
