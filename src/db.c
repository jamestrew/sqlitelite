#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dev/logging.h"
#include "input_handler.h"
#include "tables.h"

Logger *logger;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("Must supply a database filename.");
    exit(EXIT_FAILURE);
  }
  int loglevel = WARN;
  if (argc >= 3) {
    loglevel = atol(argv[2]);
  }
  logger = getLogger(loglevel); // TODO: get from flag
  initLogSession(logger);

  char *filename = argv[1];
  InputBuffer *inputBuffer = newInputBuffer();
  Table *table = dbOpen(filename);

  while (true) {
    printPrompt();
    readInput(inputBuffer);

    if (inputBuffer->buffer[0] == '.') {
      switch (doMetaCommand(inputBuffer, table)) {
      case (META_COMMAND_SUCCES):
        continue;
      case (META_COMMAND_UNRECOGNIZED):
        printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
        continue;
      }
    } else {
      Statement statement;
      switch (prepareStatement(inputBuffer, &statement)) {
      case (PREPARE_SUCCESS):
        executeStatement(&statement, table);
        break;
      case (PREPARE_UNRECOGNIZED):
        printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
        continue;
      case (PREPARE_SYNTAX_ERROR):
        puts("Syntax error: missing arguments");
        continue;
      case (PREPARE_STRING_TOO_LONG):
        puts("Statement strings too long");
        continue;
      case (PREPARE_INVALID_ID):
        puts("ID must be a positive integer.");
        continue;
      }
    }
  }
}
