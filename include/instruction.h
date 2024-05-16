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

    uint16_t emit;
    uint16_t start;
    uint16_t exec;
    uint16_t write;
} Instruction;

void instruction_print(Instruction* i);

#endif
