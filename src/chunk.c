#include <stdlib.h>
#include "chunk.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(
      uint8_t, chunk->code, oldCapacity, chunk->capacity
    );
    chunk->lines = GROW_ARRAY(
      int, chunk->lines, oldCapacity, chunk->capacity
    );
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

// ! Replaces addConstant in the original book !
// Challenge (after Ch. 14.6):
// write a OP_CONSTANT_LONG instruction that stores
// the operand as a 24-bit number
void writeConstant(Chunk *chunk, Value value, int line) {
  writeValueArray(&chunk->constants, value);
  int constant = chunk->constants.count - 1;

  // use OP_CONSTANT_SHORT if the ValueArray index
  // can fit inside a uint8_t
  if (constant < sizeof(uint8_t)) {
    writeChunk(chunk, OP_CONSTANT_SHORT, line);
    writeChunk(chunk, constant, line);
    return;
  } 

  // Otherwise use a OP_CONSTANT_LONG
  uint8_t b1 = constant >> 8;
  uint8_t b2 = constant ^ b2 << 8;

  writeChunk(chunk, OP_CONSTANT_LONG, line);
  writeChunk(chunk, b1, line);
  writeChunk(chunk, b2, line);
 }


