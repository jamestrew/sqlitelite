#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_handler.h"

void readInput(InputBuffer *const inputBuffer) {
  ssize_t bytesRead =
      getline(&(inputBuffer->buffer), &(inputBuffer->buffer_length), stdin);

  if (bytesRead <= 0) {
    puts("Error reading input");
    exit(EXIT_FAILURE);
  }
  inputBuffer->input_length = bytesRead - 1;
  inputBuffer->buffer[bytesRead - 1] = '\0';
}

InputBuffer *newInputBuffer() {
  InputBuffer *inputBuffer = malloc(sizeof(InputBuffer));
  if (inputBuffer != NULL) {
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
  }
  return inputBuffer;
}

void printPrompt() { printf("%s", "db > "); }

void closeInputBuffer(InputBuffer *const inputBuffer) {
  free(inputBuffer->buffer);
  free(inputBuffer);
}

MetaCommandResult doMetaCommand(InputBuffer *const inputBuffer) {
  if (strcmp(inputBuffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}

PrepareResult prepareStatement(InputBuffer *const inputBuffer,
                               Statement *statement) {
  if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  } else if (strncmp(inputBuffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED;
}

void executeStatement(Statement *const statement) {
  switch (statement->type) {
  case (STATEMENT_INSERT):
    puts("Doing some inserting");
    // TODO: insert handling
    break;
  case (STATEMENT_SELECT):
    puts("Doing some selecting");
    // TODO: select handling
    break;
  }
  puts("Executed.");
}
