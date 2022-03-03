#include <stdint.h>
#include <stdlib.h>

#include "cursor.h"
#include "tables.h"

Cursor *tableStart(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  if (cursor != NULL) {
    cursor->table = table;
    cursor->rowNum = 0;
    cursor->endOfTable = (table->numRows == 0);
  }
  return cursor;
}

Cursor *tableEnd(Table *table) {
  Cursor *cursor = malloc(sizeof(Cursor));
  if (cursor != NULL) {
    cursor->table = table;
    cursor->rowNum = table->numRows;
    cursor->endOfTable = true;
  }
  return cursor;
}

void *cursorValue(Cursor *cursor) {
  uint32_t pageNum = cursor->rowNum / ROWS_PER_PAGE;
  void *page = getPage(cursor->table->pager, pageNum);
  if (page == NULL) {
    page = cursor->table->pager->pages[pageNum] = malloc(PAGE_SIZE);
  }
  uint32_t rowOffset = cursor->rowNum % ROWS_PER_PAGE;
  uint32_t byteOffet = rowOffset * ROW_SIZE;
  return page + byteOffet;
}

void cursorAdvance(Cursor *cursor) {
  cursor->rowNum++;
  if (cursor->rowNum >= cursor->table->numRows) {
    cursor->endOfTable = true;
  }
}
