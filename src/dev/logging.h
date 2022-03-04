#ifndef JT_LOGGING
#define JT_LOGGING

#include <stdio.h>

#define MSG_SIZE 255

typedef enum {
  NOT_SET = 10,
  DEBUG = 20,
  INFO = 30,
  WARN = 40,
  ERROR = 50,
  CRITICAL = 60
} logLevel;

typedef struct {
  int level;
  char *directory;
  char *dtFormat;
  FILE *logFile;
} Logger;

Logger *setLogger(logLevel level);
void initLogSession(Logger *logger);
void killLogSession(Logger *logger);

void getDateTime(char *format, char *dtStr);
// void writeLog(char *msg, FILE logFile);
void generateMsg(char *msg);

// void printPrompt();
void debug(char *const msg, Logger *logger);
void info(char *const msg, Logger *logger);
void warn(char *const msg, Logger *logger);
void error(char *const msg, Logger *logger);
void critical(char *const msg, Logger *logger);

#endif
