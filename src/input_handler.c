#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_handler.h"
#include "tables.h"
#include "cursor.h"
#include "dev/logging.h"

extern Logger *logger;

void readInput(InputBuffer *const inputBuffer) {
  debug(logger, "readINput()");
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
  debug(logger, "newInputBuffer()");
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
  debug(logger, "closeInputBuffer()");
  free(inputBuffer->buffer);
  free(inputBuffer);
}

MetaCommandResult doMetaCommand(InputBuffer *const inputBuffer, Table *table) {
  debug(logger, "doMetaCommand()");
  if (strcmp(inputBuffer->buffer, ".exit") == 0) {
    dbClose(table);
    killLogSession(logger);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}

PrepareResult prepareInsert(InputBuffer *inputBuffer, Statement *statement) {
  debug(logger, "prepareInsert()");
  statement->type = STATEMENT_INSERT;

  char *keyword = strtok(inputBuffer->buffer, " ");
  char *id_str = strtok(NULL, " ");
  char *username = strtok(NULL, " ");
  char *email = strtok(NULL, " ");

  if (atoi(id_str) <= 0) {
    return PREPARE_INVALID_ID;
  }
  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }
  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    return PREPARE_STRING_TOO_LONG;
  }

  statement->insertRow.id = atoi(id_str);
  strcpy(statement->insertRow.username, username);
  strcpy(statement->insertRow.email, email);
  return PREPARE_SUCCESS;
}

PrepareResult prepareStatement(InputBuffer *inputBuffer, Statement *statement) {
  debug(logger, "prepareStatement()");
  if (strncmp(inputBuffer->buffer, "insert", 6) == 0) {
    return prepareInsert(inputBuffer, statement);
  } else if (strncmp(inputBuffer->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED;
}

void executeStatement(Statement *const statement, Table *const table) {
  debug(logger, "executeStatement()");
  ExecuteResult result;
  switch (statement->type) {
  case (STATEMENT_INSERT):
    result = executeInsert(statement, table);
    break;
  case (STATEMENT_SELECT):
    result = executeSelect(statement, table);
    break;
  }

  switch (result) {
  case (EXECUTE_SUCCESS):
    puts("Executed.");
    break;
  case (EXECUTE_TABLE_FULL):
    puts("Error: Table full.");
    break;
  }
}

ExecuteResult executeInsert(Statement *statement, Table *const table) {
  debug(logger, "executeInsert()");
  if (table->numRows >= TABLE_MAX_ROWS) {
    return EXECUTE_TABLE_FULL;
  }

  Row *rowInsert = &(statement->insertRow);
  Cursor *cursor = tableEnd(table);
  serializeRow(rowInsert, cursorValue(cursor));
  table->numRows++;
  free(cursor);
  return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement *statement, Table *const table) {
  debug(logger, "executeSelect()");
  Row row;
  Cursor *cursor = tableStart(table);
  while (!cursor->endOfTable) {
    deserializeRow(cursorValue(cursor), &row);
    cursorAdvance(cursor);
    printRow(&row);
  }
  free(cursor);
  return EXECUTE_SUCCESS;
}

void printRow(const Row *const row) {
  printf("\t%d\t%s\t%s\n", row->id, row->username, row->email);
}
