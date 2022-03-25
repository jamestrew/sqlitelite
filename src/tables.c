#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pager.h"
#include "tables.h"
#include "dev/logging.h"

extern Logger *logger;

const uint32_t ID_SIZE = SIZE_OF_ATTR(Row, id);
const uint32_t USERNAME_SIZE = SIZE_OF_ATTR(Row, username);
const uint32_t EMAIL_SIZE = SIZE_OF_ATTR(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

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
    table->numRows = pager->fileLength / ROW_SIZE;
    table->pager = pager;
  }
  return table;
}

void dbClose(Table *table) {
  debug(logger, "dbClose()");
  Pager *pager = table->pager;
  uint32_t numFullPages = table->numRows / ROWS_PER_PAGE;

  for (uint32_t i = 0; i < numFullPages; ++i) {
    if (pager->pages[i] == NULL) {
      continue;
    }
    pagerFlush(pager, i, PAGE_SIZE);
    free(pager->pages[i]);
    pager->pages[i] = NULL;
  }

  uint32_t numAdditionalRows = table->numRows % ROWS_PER_PAGE;
  if (numAdditionalRows > 0) {
    if (pager->pages[numFullPages] != NULL) {
      pagerFlush(pager, numFullPages, numAdditionalRows * ROW_SIZE);
      free(pager->pages[numFullPages]);
      pager->pages[numFullPages] = NULL;
    }
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
