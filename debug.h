#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassemble_chunk(Chunk* chunk, const char* name);
uint16_t disassemble_instruction(Chunk* chunk, uint16_t offset);

#endif