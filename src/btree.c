#include <stdint.h>
#include <stdlib.h>

#include "btree.h"
#include "dev/logging.h"

extern Logger *logger;

uint32_t *leafNodeNumCells(void *node) {
  debug(logger, "leafNodeNumCells()");
  return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *leafNodeCell(void *node, uint32_t cellNum) {
  debug(logger, "leafNodeCell()");
  return node + LEAF_NODE_HEADER_SIZE + (cellNum * LEAF_NODE_CELL_SIZE);
}

uint32_t *leafNodeKey(void *node, uint32_t cellNum) {
  debug(logger, "leafNodeKey()");
  return leafNodeCell(node, cellNum);
}

void *leafNodeValue(void *node, uint32_t cellNum) {
  debug(logger, "leafNodeValue()");
  return leafNodeCell(node, cellNum) + LEAF_NODE_KEY_SIZE;
}

NodeType getNodeType(void *node) {
  debug(logger, "getNodeType()");
  uint8_t value = *((uint8_t *)(node + NODE_TYPE_OFFSET));
  return (NodeType)value;
}

void setNodeType(void *node, NodeType type) {
  debug(logger, "setNodeType()");
  uint8_t value = type;
  *((uint8_t *)(node + NODE_TYPE_OFFSET)) = value;
}

void initializeLeafNode(void *node) {
  debug(logger, "initializeLeafNode()");
  setNodeType(node, NODE_LEAF);
  *leafNodeNumCells(node) = 0;
}
