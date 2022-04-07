#ifndef JT_CURSOR
#define JT_CURSOR

#include <stdbool.h>
#include <stdint.h>

#include "tables.h"

typedef struct {
  Table *table;
  uint32_t pageNum;
  uint32_t cellNum;
  bool endOfTable;
} Cursor;

Cursor *tableStart(Table *table);
Cursor *tableEnd(Table *table);

void *cursorValue(Cursor *cursor);
void cursorAdvance(Cursor *cursor);

// TODO: belong here?
void leafNodeInsert(Cursor *cursor, uint32_t key, Row *value);

#endif

