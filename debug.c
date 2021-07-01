#include <stdio.h>

#include "debug.h"
#include "value.h"

uint32_t PREVIOUS_LINE;

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);

  for (uint16_t offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static uint16_t simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static uint16_t constantInstruction(const char* name, Chunk* chunk, uint16_t offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static uint16_t constant_long_instruction(const char* name, Chunk* chunk, uint16_t offset) {
  uint32_t constant = chunk->code[offset+1] + (chunk->code[offset+2]<<8) + (chunk->code[offset+3]<<16);
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 4;
}

uint16_t getLine(Line* lines, uint16_t index, uint16_t offset) {
  uint16_t n = 0; 
  for (uint16_t i = 1; i <= index; i++) {
    n += lines[i].times;
    if (offset < n) return lines[i].line_number;
  }
}

uint16_t disassembleInstruction(Chunk* chunk, uint16_t offset) {
  printf("%04d ", offset);
  uint16_t line = getLine(chunk->lines, chunk->line_index, offset);
  if (line == PREVIOUS_LINE) printf(" ||| ");
  else printf("%4d ", line);
  PREVIOUS_LINE = line;
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
      return constant_long_instruction("OP_CONSTANT_LONG", chunk, offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}