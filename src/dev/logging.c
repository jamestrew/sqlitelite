#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logging.h"

Logger *getLogger(logLevel level) {
  Logger *logger = malloc(sizeof(Logger));

  if (logger == NULL) {
    puts("Failed to allocate logger");
    exit(EXIT_FAILURE);
  }
  logger->level = level;
  logger->directory = "/tmp/sqlitelite.log";
  logger->dtFormat = "MM-DD-YYYY HH:MM:ss {msg}"; // TODO: ignoring for now

  return logger;
}

void initLogSession(Logger *logger) {
  logger->logFile = fopen(logger->directory, "a");
  char msg[MSG_SIZE];
  getDateTime(logger->dtFormat, msg);
  strcat(msg, "Session started\n");
  fputs(msg, logger->logFile);
}

void killLogSession(Logger *logger) {
  char msg[MSG_SIZE];
  getDateTime(logger->dtFormat, msg);
  strcat(msg, "Session ended\n");
  fputs(msg, logger->logFile);
  fclose(logger->logFile);
  free(logger);
}

void getDateTime(char *format, char *dtStr) {
  time_t rawtime;
  time(&rawtime);
  struct tm *info = localtime(&rawtime);

  // TODO: handle different formats
  sprintf(dtStr, "[%02d/%02d/%d %02d:%02d:%02d] ", (info->tm_mon + 1),
          info->tm_mday, (info->tm_year + 1900), info->tm_hour, info->tm_min,
          info->tm_sec);
}

void generateMsg(char *msg) {
  // use to refactor code below
}

void debug(char *const msg, Logger *logger) {
  if (logger->level > 10) {
    return;
  }
  char logMsg[MSG_SIZE];
  getDateTime(logger->dtFormat, logMsg);
  strcat(logMsg, "[DEBUG] ");
  strcat(logMsg, msg);
  fprintf(logger->logFile, "%s\n", logMsg);
}

void info(char *const msg, Logger *logger) {
  if (logger->level > 20) {
    return;
  }
  char logMsg[MSG_SIZE];
  getDateTime(logger->dtFormat, logMsg);
  strcat(logMsg, "[INFO] ");
  strcat(logMsg, msg);
  fprintf(logger->logFile, "%s\n", logMsg);
}

void warn(char *const msg, Logger *logger) {
  if (logger->level > 30) {
    return;
  }
  char logMsg[MSG_SIZE];
  getDateTime(logger->dtFormat, logMsg);
  strcat(logMsg, "[WARN] ");
  strcat(logMsg, msg);
  fprintf(logger->logFile, "%s\n", logMsg);
}

void error(char *const msg, Logger *logger) {
  if (logger->level > 40) {
    return;
  }
  char logMsg[MSG_SIZE];
  getDateTime(logger->dtFormat, logMsg);
  strcat(logMsg, "[ERROR] ");
  strcat(logMsg, msg);
  fprintf(logger->logFile, "%s\n", logMsg);

  // printPrompt();
  printf("%s", "\033[0;31m");
  puts(msg);
  printf("%s", "\033[0m");
}

void critical(char *const msg, Logger *logger) {
  if (logger->level > 50) {
    return;
  }
  char logMsg[MSG_SIZE];
  getDateTime(logger->dtFormat, logMsg);
  strcat(logMsg, "[CRIT] ");
  strcat(logMsg, msg);
  fprintf(logger->logFile, "%s\n", logMsg);

  // printPrompt();
  printf("%s", "\033[0;31m");
  puts(msg);
  printf("%s", "\033[0m");
}

// void printPrompt() { printf("%s", "db > "); }
