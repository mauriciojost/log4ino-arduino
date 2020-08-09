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

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL Fine
#endif // DEFAULT_LOG_LEVEL

#ifndef MAX_LOG_MSG_LENGTH
#define MAX_LOG_MSG_LENGTH 16
#endif // MAX_LOG_MSG_LENGTH

#define LOG_CLASS "LG"
#define LOG_UNIT_EXPR_LEN 4

#ifndef MAX_LOG_OPTIONS_RULES
#define MAX_LOG_OPTIONS_RULES 8
#endif // MAX_LOG_OPTIONS_RULES

char logOptions[(MAX_LOG_OPTIONS_RULES * LOG_UNIT_EXPR_LEN) + 1];
char *logStaticBuffer = NULL;

void disableLogOptions() {
  strcpy(logOptions, " ");
}

bool hasToLog(LogLevel l, const char* clz) {

  if (getLogOptions() == NULL) {
    return (l >= DEFAULT_LOG_LEVEL);
  }

  for (int p = 0; p < strlen(getLogOptions()); p += LOG_UNIT_EXPR_LEN) {
    char optClz0 = logOptions[p+0];
    char optClz1 = logOptions[p+1];
    char optLogLvl = logOptions[p+2];
    LogLevel lg;
    switch (optLogLvl) {
      case 'f':
      case 'F':
        lg = Fine;
        break;
      case 'd':
      case 'D':
        lg = Debug;
        break;
      case 'i':
      case 'I':
        lg = Info;
        break;
      case 'w':
      case 'W':
        lg = Warn;
        break;
      case 'e':
      case 'E':
        lg = Error;
        break;
      case 'u':
      case 'U':
        lg = User;
        break;
      default:
        lg = (LogLevel)(optLogLvl - '0');
        break;
    }
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
  return (l >= DEFAULT_LOG_LEVEL);
}

void setLogOptions(const char *opts) {
  if (opts == NULL) {
    disableLogOptions();
  } else {
    if (strlen(opts) % 4 == 0) {
      strncpy(logOptions, opts, MAX_LOG_OPTIONS_RULES * LOG_UNIT_EXPR_LEN);
      logOptions[MAX_LOG_OPTIONS_RULES * LOG_UNIT_EXPR_LEN] = 0;
    } else {
      log("LG", User, "Invalid log options");
      disableLogOptions();
    }
  }
}

const char* getLogOptions() {
#ifdef LOG_FIXED_OPTIONS
  return LOG_FIXED_OPTIONS;
#else // LOG_FIXED_OPTIONS
  if (logOptions[0] == ' ' || logOptions[0] == 0 ) {
    return NULL;
  } else {
    unsigned int len = strlen(logOptions);
    if (len < 1 || len % 4 != 0) {
      return NULL;
    } else {
      return logOptions;
    }
  }
#endif // LOG_FIXED_OPTIONS
}

#ifdef YES_DEBUG

const char *logLevelStr[6] = {"F", "D", "I", "W", "E", "U"};

#ifdef X86_64
#include <log4ino/Colors.h>
const char *logLevelStrRich[6] = {KYEL "FINE" KNRM, KYEL "DEBUG" KNRM, KBLU "INFO " KNRM, KMAG "WARN " KNRM, KRED "ERROR" KNRM, KBLU "USER" KNRM};
#endif // X86_64


void (*prntFunc)(const char *msg, const char *clz, LogLevel l, bool newline) = NULL;
#endif // YES_DEBUG

void setupLog(void (*prnt)(const char *msg, const char *clz, LogLevel l, bool newline)) {
#ifdef YES_DEBUG
  prnt("-U-\n", LOG_CLASS, Debug, true);
  prntFunc = prnt;
  disableLogOptions();
  logStaticBuffer = new char[MAX_LOG_MSG_LENGTH];
#endif // YES_DEBUG
}

void log(const char *clz, LogLevel l, const char *format, ...) {
#ifdef YES_DEBUG
  if (prntFunc == NULL) // if not initialized
    return;

  if (hasToLog(l, clz)) {

    snprintf(logStaticBuffer, MAX_LOG_MSG_LENGTH, "%s %s ", clz, logLevelStr[l]);
    logStaticBuffer[MAX_LOG_MSG_LENGTH - 1] = 0;
    prntFunc(logStaticBuffer, clz, l, true);
#ifdef X86_64 // print anyway
    printf("[%8.8s] [%s]: ", clz, logLevelStrRich[l]);
#endif // X86_64

    va_list args;
    va_start(args, format);
    vsnprintf(logStaticBuffer, MAX_LOG_MSG_LENGTH, format, args);
    va_end(args);
    strncat(logStaticBuffer, "\n", MAX_LOG_MSG_LENGTH);
    logStaticBuffer[MAX_LOG_MSG_LENGTH - 1] = 0;
    logStaticBuffer[MAX_LOG_MSG_LENGTH - 2] = '\n';
    prntFunc(logStaticBuffer, clz, l, false);
#ifdef X86_64 // print anyway
    printf("%s\n", logStaticBuffer);
#endif // X86_64
  }
#endif // YES_DEBUG
}


void logHex(const char *clz, LogLevel l, const unsigned char *buf, int bytes) {
#ifdef YES_DEBUG
  char buffer[MAX_LOG_MSG_LENGTH];
  char val[4];
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
      prntFunc(clz, clz, l, true);
      prntFunc(" ", clz, l, false);
      prntFunc(logLevelStr[l], clz, l, false);
      prntFunc(" ", clz, l, false);
      prntFunc(raw, clz, l, false);
      prntFunc("\n", clz, l, false);
  	}
  }
#endif // YES_DEBUG
}
