#include <stdio.h>

#include "debug.h"
#include "value.h"

uint32_t PREVIOUS_LINE;

void disassemble_chunk(Chunk* chunk, const char* name) {
  printf("== %s ==\n", name);

  for (uint16_t offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

static uint16_t simple_instruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static uint16_t constant_instruction(const char* name, Chunk* chunk, uint16_t offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static uint16_t constant_long_instruction(const char* name, Chunk* chunk, uint16_t offset) {
  uint32_t constant = chunk->code[offset+1] + (chunk->code[offset+2]<<8) + (chunk->code[offset+3]<<16);
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 4;
}

uint16_t get_line(Line* lines, uint16_t index, uint16_t offset) {
  uint16_t n = 0; 
  for (uint16_t i = 1; i <= index; i++) {
    n += lines[i].times;
    if (offset < n) return lines[i].line_number;
  }
}

uint16_t disassemble_instruction(Chunk* chunk, uint16_t offset) {
  printf("%04d ", offset);
  uint16_t line = get_line(chunk->lines, chunk->line_index, offset);
  if (line == PREVIOUS_LINE) printf(" ||| ");
  else printf("%4d ", line);
  PREVIOUS_LINE = line;
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
      return constant_long_instruction("OP_CONSTANT_LONG", chunk, offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}