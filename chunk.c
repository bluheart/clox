#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->line_index = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  init_value_array(&chunk->constants);
}

void free_chunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}

void write_chunk(Chunk* chunk, uint8_t byte, uint16_t line) {
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

void write_constant(Chunk* chunk, Value value, uint16_t line) {
  int offset = add_constant(chunk, value);
  if (offset < 256) {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, offset, line);
  }
  else {
    write_chunk(chunk, OP_CONSTANT_LONG, line);
    write_chunk(chunk, offset & 0xff, line);
    write_chunk(chunk, (offset & 0xff00) >> 8, line);
    write_chunk(chunk, (offset & 0xff0000) >> 16, line);
  }
}

uint16_t add_constant(Chunk* chunk, Value value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}