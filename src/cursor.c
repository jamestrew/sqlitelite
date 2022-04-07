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

Cursor *tableFind(Table * table, uint32_t key) {
  debug(logger, "tableFind()");
  void *rootNode = getPage(table->pager, table->rootPageNum);
  if (getNodeType(rootNode) == NODE_LEAF) {
    return leafNodeFind(table, key);
  } else {
    critical(logger, "TO BE IMPLEMENTED: searching internal node");
    exit(EXIT_FAILURE);
  }
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
  debug(logger, "leafNodeInsert()");
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

Cursor *leafNodeFind(Table *table, uint32_t key) {
  debug(logger, "leafNodeFind()");
  void *node = getPage(table->pager, table->rootPageNum);
  uint32_t numCells = *leafNodeNumCells(node);

  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->pageNum = table->rootPageNum;

  uint32_t minIndex = 0;
  uint32_t maxIndexPlusOne = numCells;
  while (maxIndexPlusOne != minIndex) {
    uint32_t index = (minIndex + maxIndexPlusOne) / 2;
    uint32_t indexKey = *leafNodeKey(node, index);
    if (key == indexKey) {
      cursor->cellNum = index;
      return cursor;
    }
    if (key < indexKey) {
      maxIndexPlusOne = index;
    } else {
      minIndex = index + 1;
    }
  }

  cursor->cellNum = minIndex;
  return cursor;
}
