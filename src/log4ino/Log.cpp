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

#ifndef MAX_LOG_MSG_LENGTH
#define MAX_LOG_MSG_LENGTH 16
#endif // MAX_LOG_MSG_LENGTH

#define LOG_CLASS "LG"
#define LOG_UNIT_EXPR_LEN 4
char logLevel = LOG_LEVEL;
const char* logOptions = NULL;

bool hasToLog(LogLevel l, const char* clz) {

  if (logOptions == NULL || strlen(logOptions) % 4 != 0) {
    return (l >= logLevel);
  }

  for (int p = 0; p < strlen(logOptions); p += LOG_UNIT_EXPR_LEN) {
    char optClz0 = logOptions[p+0];
    char optClz1 = logOptions[p+1];
    LogLevel lg = (LogLevel)(logOptions[p+2] - '0');
    if(clz[0] == optClz0 && clz[1] == optClz1) { // direct match
      return (l >= lg);
    } else if (clz[0] == '?' && clz[1] == optClz1) { // one char match
      return (l >= lg);
    } else if (clz[0] == optClz0 && clz[1] == '?') { // one char match
      return (l >= lg);
    } else if ('?' == optClz0 && '?' == optClz1) { // no char match
      return (l >= lg);
    }
  }
  return true;
}

void setLogOptions(const char *opts) {
  logOptions = opts;
}

const char* getLogOptions() {
  return logOptions;
}

#ifdef ARDUINO

// ARDUINO, SO ON-BOARD EXECUTION
#ifdef YES_DEBUG
const char *logLevelStr[5] = {"D", "I", "W", "E", "U"};
void (*prntFunc)(const char *msg, const char *clz, LogLevel l) = NULL;
#endif // YES_DEBUG

void setLogLevel(char level) { 
#ifdef YES_DEBUG
  logLevel = level;
#endif // YES_DEBUG
}
char getLogLevel(){
#ifdef YES_DEBUG
  return logLevel;
#endif // YES_DEBUG
}

void setupLog(void (*prnt)(const char *msg, const char *clz, LogLevel l)) {
#ifdef YES_DEBUG
  prnt("-U-", LOG_CLASS, Debug);
  prntFunc = prnt;
  logOptions = NULL;
#endif // YES_DEBUG
}

void log(const char *clz, LogLevel l, const char *format, ...) {
#ifdef YES_DEBUG
  if (hasToLog(l, clz)) {

    char buffer[MAX_LOG_MSG_LENGTH];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, MAX_LOG_MSG_LENGTH, format, args);
    va_end(args);
    buffer[MAX_LOG_MSG_LENGTH - 1] = 0;

    char bufferTotal[MAX_LOG_MSG_LENGTH];
    snprintf(bufferTotal, MAX_LOG_MSG_LENGTH, "%s %s %s\n", clz, logLevelStr[l],
             buffer);
    bufferTotal[MAX_LOG_MSG_LENGTH - 1] = 0;

    if (prntFunc != NULL) {
      prntFunc(bufferTotal, clz, l);
    }
  }
#endif // YES_DEBUG
}


void logHex(const char *clz, LogLevel l, const unsigned char *buf, int bytes) {
#ifdef YES_DEBUG
  char buffer[MAX_LOG_MSG_LENGTH];
  char val[3];
  buffer[0] = 0;
  for (int i = 0; i < bytes; i++) {
    sprintf(val, "%.2x\n", buf[i]);
    strncat(buffer, val, MAX_LOG_MSG_LENGTH);
  }
  buffer[MAX_LOG_MSG_LENGTH - 1] = 0;
  log(clz, l, buffer);
#endif // YES_DEBUG
}

void logRaw(const char *clz, LogLevel l, const char *raw) {
#ifdef YES_DEBUG
  if (hasToLog(l, clz)) {
  	if (prntFunc != NULL) {
      prntFunc(clz, clz, l);
      prntFunc(" ", clz, l);
      prntFunc(logLevelStr[l], clz, l);
      prntFunc(" ", clz, l);
      prntFunc(raw, clz, l);
      prntFunc("\n", clz, l);
  	}
  }
#endif // YES_DEBUG
}

#endif

#ifdef X86_64

#include <log4ino/Colors.h>

const char *logLevelStr[4] = {KYEL "DEBUG" KNRM, KBLU "INFO " KNRM,
                              KMAG "WARN " KNRM, KRED "ERROR" KNRM};

void setupLog(void (*prnt)(const char *msg, const char *clz, LogLevel l)) { }

void setLogLevel(char level) {
  logLevel = level;
}
char getLogLevel(){return logLevel;}

void log(const char *clz, LogLevel l, const char *format, ...) {
  if (hasToLog(l, clz)) {
    char buffer[MAX_LOG_MSG_LENGTH];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, MAX_LOG_MSG_LENGTH, format, args);
    buffer[MAX_LOG_MSG_LENGTH - 1] = 0;
    printf("[%8.8s] [%s]: %s\n", clz, logLevelStr[l], buffer);
    va_end(args);
  }
}

void logHex(const char *clz, LogLevel l, const unsigned char *buf, int bytes) {
  char buffer[MAX_LOG_MSG_LENGTH];
  char val[3];
  buffer[0] = 0;
  for (int i = 0; i < bytes; i++) {
    sprintf(val, "%.2x", buf[i]);
    strncat(buffer, val, MAX_LOG_MSG_LENGTH);
  }
  buffer[MAX_LOG_MSG_LENGTH - 1] = 0;
  log(clz, l, buffer);
}

void logRaw(const char *clz, LogLevel l, const char *raw) {
  if (hasToLog(l, clz)) {
    printf("[%8.8s] [%s]: %s\n", clz, logLevelStr[l], raw);
  }
}

#endif // X86_64
