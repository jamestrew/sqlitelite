#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pager.h"
#include "dev/logging.h"

extern Logger *logger;

Pager *pagerOpen(const char *filename) {
  debug(logger, "pagerOpen()");
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

  if (fd == -1) {
    printf("Unable to open %s.\n", filename);
    exit(EXIT_FAILURE);
  }

  off_t fileLength = lseek(fd, 0, SEEK_END);
  Pager *pager = malloc(sizeof(Pager));
  pager->fileDescriptor = fd;
  pager->fileLength = fileLength;

  for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
    pager->pages[i] = NULL;
  }

  return pager;
}

void *getPage(Pager *pager, uint32_t pageNum) {
  debug(logger, "getPage()");
  if (pageNum > TABLE_MAX_PAGES) {
    printf("Tried to fetch page number out of bounds. %d\n", TABLE_MAX_PAGES);
    exit(EXIT_FAILURE);
  }

  if (pager->pages[pageNum] == NULL) {
    void *page = malloc(PAGE_SIZE);
    uint32_t numPages = pager->fileLength / PAGE_SIZE;

    if (pager->fileLength % PAGE_SIZE) {
      numPages++;
    }

    if (pageNum <= numPages) {
      lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);
      ssize_t bytesRead = read(pager->fileDescriptor, page, PAGE_SIZE);
      if (bytesRead == -1) {
        printf("Error reading the file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }
    pager->pages[pageNum] = page;
  }
  return pager->pages[pageNum];
}

void pagerFlush(Pager *pager, uint32_t pageNum, uint32_t size) {
  debug(logger, "pagerFlush()");
  if (pager->pages[pageNum] == NULL) {
    puts("Tried to flush null page");
    exit(EXIT_FAILURE);
  }

  off_t offset = lseek(pager->fileDescriptor, pageNum * PAGE_SIZE, SEEK_SET);
  if (offset == 1) {
    printf("Error seeking: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  ssize_t bytesWritten =
      write(pager->fileDescriptor, pager->pages[pageNum], size);
  if (bytesWritten == -1) {
    printf("Error writing: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}
