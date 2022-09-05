#ifndef clox_debug_h
#define clox_debug_h

// An assembler is an old-school program that takes a file
// containing human-readable names for CPU instructions
// like "ADD" and "MULT" and translates them to their binary
// machine code equivalent. A disassembler goes in the other
// direction - given a blob of machine code, it spits out a 
// textual listing of the instructuins

#include "common.h"
#include "chunk.h"

void disassembleChunk(Chunk *chunk, const char *name);
int disassembleInstruction(Chunk *chunk, int offset);

#endif
