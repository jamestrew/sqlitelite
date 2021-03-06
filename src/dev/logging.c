
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logging.h"

Logger *getLogger(LogLevel level) {
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
  sprintf(dtStr, "[%02d/%02d/%d %02d:%02d:%02d]", (info->tm_mon + 1),
          info->tm_mday, (info->tm_year + 1900), info->tm_hour, info->tm_min,
          info->tm_sec);
}

void printLog(char *msg, char *msgColor) {
  printf("%s%s%s\n", msgColor, msg, MSG_COLOR_NONE);
}

void debug(Logger *logger, char *const format, ...) {
  if (logger->level > 20) {
    return;
  }
  char logformat[MSG_SIZE];
  char dtStr[strlen(logger->dtFormat)];
  getDateTime(logger->dtFormat, dtStr);

  va_list args;
  va_start(args, format);
  vsprintf(logformat, format, args);
  va_end(args);

  fprintf(logger->logFile, "[DEBUG] %s %s\n", dtStr, logformat);
}

void warn(Logger *logger, char *const format, ...) {
  if (logger->level > 40) {
    return;
  }
  char logformat[MSG_SIZE];
  char dtStr[strlen(logger->dtFormat)];
  getDateTime(logger->dtFormat, dtStr);

  va_list args;
  va_start(args, format);
  vsprintf(logformat, format, args);
  va_end(args);

  fprintf(logger->logFile, "[CRIT] %s %s\n", dtStr, logformat);
  printLog(logformat, MSG_COLOR_NONE);
}

void critical(Logger *logger, char *const format, ...) {
  if (logger->level > 60) {
    return;
  }
  char logformat[MSG_SIZE];
  char dtStr[strlen(logger->dtFormat)];
  getDateTime(logger->dtFormat, dtStr);

  va_list args;
  va_start(args, format);
  vsprintf(logformat, format, args);
  va_end(args);

  fprintf(logger->logFile, "[CRIT] %s %s\n", dtStr, logformat);
  printLog(logformat, MSG_COLOR_RED);
}

#ifdef LOGGING_TESTING
// run with `gcc srv/dev/logging.c -o logging -D LOGGING_TESTING`
int main() {
  puts("LOGGIN_TESTING");
  Logger *logger = getLogger(DEBUG);
  initLogSession(logger);
  debug(logger, "testing out debug");
  critical(logger, "coolest number %d", 420);
  critical(logger, "some random string %s", "foobarbaz");
  warn(logger, "this is a warning");
  killLogSession(logger);
}
#endif
