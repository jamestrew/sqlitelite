#ifndef JT_PAGER
#define JT_PAGER

#include <stdint.h>

#define TABLE_MAX_PAGES 100

extern const uint32_t PAGE_SIZE;

typedef struct {
  int fileDescriptor;
  uint32_t fileLength;
  void *pages[TABLE_MAX_PAGES];
} Pager;


Pager *pagerOpen(const char *filename);
void *getPage(Pager *pager, uint32_t pageNum);
void pagerFlush(Pager *pager, uint32_t pageNum, uint32_t size);

#endif
