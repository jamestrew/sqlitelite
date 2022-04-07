#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "cursor.h"
#include "dev/logging.h"
#include "tables.h"

extern Logger *logger;

Cursor *tableStart(Table *table) {
  debug(logger, "tableStart()");
  Cursor *cursor = malloc(sizeof(Cursor));
  if (cursor != NULL) {
    cursor->table = table;
    cursor->pageNum = table->rootPageNum;
    cursor->cellNum = 0;

    void *rootNode = getPage(table->pager, cursor->pageNum);
    cursor->endOfTable = *leafNodeNumCells(rootNode) == 0;
  }
  return cursor;
}

Cursor *tableEnd(Table *table) {
  debug(logger, "tableEnd()");
  Cursor *cursor = malloc(sizeof(Cursor));
  if (cursor != NULL) {
    cursor->table = table;
    cursor->pageNum = table->rootPageNum;

    void *rootNode = getPage(table->pager, cursor->pageNum);
    cursor->cellNum = *leafNodeNumCells(rootNode);
    cursor->endOfTable = true;
  }
  return cursor;
}

void *cursorValue(Cursor *cursor) {
  debug(logger, "cursorValue()");
  void *page = getPage(cursor->table->pager, cursor->pageNum);
  return leafNodeValue(page, cursor->cellNum);
}

void cursorAdvance(Cursor *cursor) {
  debug(logger, "cursorAdvance()");
  void *node = getPage(cursor->table->pager, cursor->pageNum);
  cursor->cellNum += 1;
  if (cursor->cellNum >= (*leafNodeNumCells(node))) {
    cursor->endOfTable = true;
  }
}

void leafNodeInsert(Cursor *cursor, uint32_t key, Row *value) {
  void *node = getPage(cursor->table->pager, cursor->pageNum);

  uint32_t numCells = *leafNodeNumCells(node);
  if (numCells >= LEAF_NODE_MAX_CELLS) {
    critical(logger, "TO BE IMPLEMENTED: splitting of leaf node");
    exit(EXIT_FAILURE);
  }

  if (cursor->cellNum < numCells) {
    for (uint32_t i = numCells; i > cursor->cellNum; --i) {
      memcpy(leafNodeCell(node, i), leafNodeCell(node, i - 1),
             LEAF_NODE_CELL_SIZE);
    }
  }

  *leafNodeNumCells(node) += 1;
  *leafNodeKey(node, cursor->cellNum) = key;
  serializeRow(value, leafNodeValue(node, cursor->cellNum));
}
