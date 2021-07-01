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

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, uint16_t line);
void writeConstant(Chunk* chunk, Value value, uint16_t line);
uint16_t addConstant(Chunk* chunk, Value value);

#endif