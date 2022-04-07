#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pager.h"
#include "tables.h"
#include "btree.h"
#include "dev/logging.h"

extern Logger *logger;

void serializeRow(Row *source, void *destination) {
  debug(logger, "serializeRow()");
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserializeRow(void *source, Row *destination) {
  debug(logger, "deserializeRow()");
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

Table *dbOpen(const char *filename) {
  debug(logger, "dbOpen()");
  Pager *pager = pagerOpen(filename);
  Table *table = malloc(sizeof(Table));

  if (table != NULL) {
    table->rootPageNum = 0;
    table->pager = pager;
  }
  if (pager->numPages == 0) {
    // new db file
    void *rootNode = getPage(pager, 0);
    initializeLeafNode(rootNode);
  }
  return table;
}

void dbClose(Table *table) {
  debug(logger, "dbClose()");
  Pager *pager = table->pager;

  for (uint32_t i = 0; i < pager->numPages; ++i) {
    if (pager->pages[i] == NULL) {
      continue;
    }
    pagerFlush(pager, i);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
  }

  if (close(pager->fileDescriptor) == -1) {
    puts("Error closing db file.");
    exit(EXIT_FAILURE);
  }

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    void *page = pager->pages[i];
    if (page) {
      free(page);
      pager->pages[i] = NULL;
    }
  }
  free(pager);
  free(table);
}

void freeTable(Table *table) {
  debug(logger, "freeTable()");
  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    free(table->pager->pages[i]);
  }
  free(table);
}
