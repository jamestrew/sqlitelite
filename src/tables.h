#ifndef JT_TABLES
#define JT_TABLES

#include <stdint.h>

#define SIZE_OF_ATTR(struct, attr) sizeof(((struct *)0)->attr)

#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL } ExecuteResult;

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

/*
  table: users

   column         size(bytes)       offset
  -----------+------------------+---------
   id        |    4             |   0
   username  |    32            |   4
   email     |    255           |   36
   total     |    291           |   --
*/
extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;
extern const uint32_t ID_OFFSET;
extern const uint32_t USERNAME_OFFSET;
extern const uint32_t EMAIL_OFFSET;
extern const uint32_t ROW_SIZE;

extern const uint32_t PAGE_SIZE;
extern const uint32_t ROWS_PER_PAGE;
extern const uint32_t TABLE_MAX_ROWS;

typedef struct {
  uint32_t numRows;
  void *pages[TABLE_MAX_PAGES];
} Table;

void serializeRow(Row *source, void *destination);
void deserializeRow(void *source, Row *destination);
void *rowSlot(Table *table, uint32_t rowNum);

Table *newTable();
void freeTable(Table *table);

#endif
