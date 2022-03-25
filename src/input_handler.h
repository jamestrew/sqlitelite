#ifndef JT_INPUT_HANDLER
#define JT_INPUT_HANDLER

#include <stdio.h>
#include <stdlib.h>

#include "tables.h"

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCES,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED,
  PREPARE_SYNTAX_ERROR,
  PREPARE_STRING_TOO_LONG,
  PREPARE_INVALID_ID
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
  Row insertRow;
} Statement;

InputBuffer *newInputBuffer();
void printPrompt();
void readInput(InputBuffer *const inputBuffer);
void closeInputBuffer(InputBuffer *const inputBuffer);
MetaCommandResult doMetaCommand(InputBuffer *const inputBuffer, Table *table);
PrepareResult prepareInput(InputBuffer *inputBuffer, Statement *statement);
PrepareResult prepareStatement(InputBuffer *inputBuffer, Statement *statement);
void executeStatement(Statement *const statement, Table *const table);

ExecuteResult executeInsert(Statement *statement, Table *const table);
ExecuteResult executeSelect(Statement *statement, Table *const table);

void printRow(const Row *const row);

#endif
