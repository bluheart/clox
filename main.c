#include "common.h"
#include <stdio.h>
#include "chunk.h"
#include "debug.h"

int main(int argc, char const *argv[])
{
    Chunk chunk;
    initChunk(&chunk);
    for (size_t i = 1; i < 300; i++) {
        writeConstant(&chunk, i*2, i);
    }
    writeChunk(&chunk, OP_RETURN, 123);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}