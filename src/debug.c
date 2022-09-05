#include <stdio.h>
#include "debug.h"

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  // Instead of incrementing offset in the loop, we let
  // `disassembleInstruction` do it for us. When we call
  // that functions, after disassembling the instruction
  // at the given offset, it returns the offset of the next
  // instruction. This is because instructions can have
  // different sizes
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int constantInstruction(const char *name, Chunk *chunk, int offset, bool isLong) {
  uint16_t constant = chunk->code[offset + 1];
  if (isLong) {
    constant = constant << 8 | chunk->code[offset + 2];
  }

  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");

  if (isLong) {
    return offset + 3;
  }
  return offset + 2;
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);

  if (offset > 0 
      &&
      chunk->lines[offset] == chunk->lines[offset - 1]
     ) {
    printf("   | ");
  } 
  else {
    printf("%4d ", chunk->lines[offset]);
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    // Constants
    case OP_CONSTANT_SHORT:
      return constantInstruction("OP_CONSTANT", chunk, offset, false);
    case OP_CONSTANT_LONG:
      return constantInstruction("OP_CONSTANT", chunk, offset, true);
    case OP_NIL:
      return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
      return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
      return simpleInstruction("OP_FALSE", offset);
    case OP_EQUAL:
      return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
      return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
      return simpleInstruction("OP_LESS", offset);

    // Binary operations
    case OP_ADD:      return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT: return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY: return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:   return simpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:      return simpleInstruction("OP_NOT", offset);
    case OP_NEGATE:   return simpleInstruction("OP_NEGATE", offset);

    // Ops
    case OP_POP: return simpleInstruction("OP_POP", offset);
    case OP_PRINT: return simpleInstruction("OP_PRINT", offset);
    case OP_DEFINE_GLOBAL: return simpleInstruction("OP_DEFINE_GLOBAL", offset);
    case OP_GET_GLOBAL: return simpleInstruction("OP_GET_GLOBAL", offset);
    case OP_SET_GLOBAL: return simpleInstruction("OP_SET_GLOBAL", offset);

    // Return & Default
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}
