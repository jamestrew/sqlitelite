#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input_handler.h"
#include "tables.h"

int main() {
  InputBuffer *inputBuffer = newInputBuffer();
  Table *table = newTable();

  while (true) {
    printPrompt();
    readInput(inputBuffer);

    if (inputBuffer->buffer[0] == '.') {
      switch (doMetaCommand(inputBuffer)) {
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
      }
    }
  }
}
