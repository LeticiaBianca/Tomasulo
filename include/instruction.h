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
} InstKind;

typedef struct {
    InstKind kind;

    // FIXME: add fields for registers and functional unit

    uint8_t emit;
    uint8_t start;
    uint8_t exec;
    uint8_t write;
} Instruction;

void instruction_fmt(FILE* stream, Instruction* i);

#endif
