#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCES,
  META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED } PrepareResult;
typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

InputBuffer *newInputBuffer();
void printPrompt();
void readInput(InputBuffer *const inputBuffer);
void closeInputBuffer(InputBuffer *const inputBuffer);
MetaCommandResult doMetaCommand(InputBuffer *const inputBuffer);
PrepareResult prepareStatement(InputBuffer *const inputBuffer,
                               Statement *statement);
void executeStatement(Statement *const statement);

#endif
