#ifndef JT_TABLES
#define JT_TABLES

#include "pager.h"
#include <stdint.h>

#define SIZE_OF_ATTR(struct, attr) sizeof(((struct *)0)->attr)

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

/*
  table: users

   column         size(bytes)       offset
  -----------+------------------+---------
   id        |    4             |   0
   username  |    32            |   4
   email     |    255           |   36
   total     |    291           |   --
*/
#define ID_SIZE 4
#define USERNAME_SIZE (COLUMN_USERNAME_SIZE + 1)
#define EMAIL_SIZE (COLUMN_EMAIL_SIZE + 1)
#define ID_OFFSET 0
#define USERNAME_OFFSET (ID_SIZE + ID_OFFSET)
#define EMAIL_OFFSET (USERNAME_OFFSET + USERNAME_SIZE)
#define ROW_SIZE (ID_SIZE + USERNAME_SIZE + EMAIL_SIZE)

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_DUPLICATE_KEY,
  EXECUTE_TABLE_FULL
} ExecuteResult;

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
  uint32_t rootPageNum;
  Pager *pager;
} Table;

void serializeRow(Row *source, void *destination);
void deserializeRow(void *source, Row *destination);

Table *dbOpen(const char *filename);
void dbClose(Table *table);
void freeTable(Table *table);

#endif
