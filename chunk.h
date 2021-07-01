#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_RETURN,
} OpCode;

typedef struct __attribute__((__packed__)) {
  uint16_t line_number;
  uint8_t times;
} Line;

typedef struct {
  uint8_t* code;
  Line* lines;
  uint16_t line_index;
  ValueArray constants;
  uint16_t count;
  uint16_t capacity;
} Chunk;

void init_chunk(Chunk* chunk);
void free_chunk(Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, uint16_t line);
void write_constant(Chunk* chunk, Value value, uint16_t line);
uint16_t add_constant(Chunk* chunk, Value value);

#endif