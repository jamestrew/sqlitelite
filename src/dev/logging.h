#ifndef JT_LOGGING
#define JT_LOGGING

#include <stdio.h>

#define MSG_SIZE 255
#define MSG_COLOR_NONE "\033[0m"
#define MSG_COLOR_RED "\033[0;31m"

typedef enum {
  NOT_SET = 10,
  DEBUG = 20,
  INFO = 30,
  WARN = 40,
  ERROR = 50,
  CRITICAL = 60
} LogLevel;

typedef struct {
  int level;
  char *directory;
  char *dtFormat;
  FILE *logFile;
} Logger;

Logger *getLogger(LogLevel level);
void initLogSession(Logger *logger);
void killLogSession(Logger *logger);

void getDateTime(char *format, char *dtStr);
void printLog(char *msg, char *msgColor);

void debug(Logger *logger, char *const format, ...);
// void info(Logger *logger, char *const format, ...);
void warn(Logger *logger, char *const format, ...);
// void error(Logger *logger, char *const format, ...);
void critical(Logger *logger, char *const format, ...);

#endif
