#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->line_index = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, uint16_t line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(Line, chunk->lines,
        oldCapacity, chunk->capacity);
  }
  if (chunk->lines[chunk->line_index].line_number == line) {
    chunk->lines[chunk->line_index].times++;
  }
  else {
    chunk->line_index++;
    chunk->lines[chunk->line_index].line_number = line;
    chunk->lines[chunk->line_index].times = 1;
  }
  chunk->code[chunk->count] = byte;
  chunk->count++;
}

void writeConstant(Chunk* chunk, Value value, uint16_t line) {
  int offset = addConstant(chunk, value);
  if (offset < 256) {
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, offset, line);
  }
  else {
    writeChunk(chunk, OP_CONSTANT_LONG, line);
    writeChunk(chunk, offset & 0xff, line);
    writeChunk(chunk, (offset & 0xff00) >> 8, line);
    writeChunk(chunk, (offset & 0xff0000) >> 16, line);
  }
}

uint16_t addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}