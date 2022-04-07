#include <stdio.h>
#include <stdint.h>

#include "../btree.h"
#include "../tables.h"
#include "logging.h"

extern Logger *logger;

void printConstants() {
  debug(logger, "printConstants()");
  puts("Constants:");
  printf("ROW_SIZE: %d\n", ROW_SIZE);
  printf("COMMON_NODE_HEADER_SIZE: %lu\n", COMMON_NODE_HEADER_SIZE);
  printf("LEAF_NODE_HEADER_SIZE: %lu\n", LEAF_NODE_HEADER_SIZE);
  printf("LEAF_NODE_CELL_SIZE: %lu\n", LEAF_NODE_CELL_SIZE);
  printf("LEAF_NODE_SPACE_FOR_CELLS: %lu\n", LEAF_NODE_SPACE_FOR_CELLS);
  printf("LEAF_NODE_MAX_CELLS: %lu\n", LEAF_NODE_MAX_CELLS);
}

void printLeafNode(void *node) {
  debug(logger, "printLeafNode()");
  puts("Tree:");
  uint32_t numCells = *leafNodeNumCells(node);
  printf("leaf (size %u)\n", numCells);
  for (uint32_t i = 0; i < numCells; ++i) {
    printf(" - %u : %u\n", i, *leafNodeKey(node, i));
  }
}
