#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.h"

typedef enum {
    Add,
    Sub,
    Mul,
    Div,
    Load,
    Store,
} InstType;

typedef struct {
    InstType type;

    // FIXME: add fields for registers and functional unit

    u8 emit;
    u8 start;
    u8 exec;
    u8 write;
} Instruction;

void instruction_fmt(FILE* stream, Instruction* i);

#endif
