#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input_handler.h"

int main() {
  InputBuffer *inputBuffer = newInputBuffer();
  while (true) {
    printPrompt();
    readInput(inputBuffer);

    // TODO: kinda want to wrap this all in an input handler function
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
        executeStatement(&statement);
        break;
      case (PREPARE_UNRECOGNIZED):
        printf("Unrecognized command '%s'.\n", inputBuffer->buffer);
        continue;
      }
    }
  }
}

