#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  META_COMMAND_SUCCES,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer *newInputBuffer();
void printPrompt();
void readInput(InputBuffer *const inputBuffer);
void closeInputBuffer(InputBuffer *const inputBuffer);
MetaCommandResult doMetaCommand(InputBuffer *const inputBuffer);
PrepareResult prepareStatement(InputBuffer *const inputBuffer,
                               Statement *statement);
void executeStatement(Statement *const statement);

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
