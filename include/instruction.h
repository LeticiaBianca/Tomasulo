#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.h"
#include "register.h"

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

	Register r1; // First operand. 
	Register r2; // Second operand.
	Register rdest;

    u8 emit;
    u8 start;
    u8 exec;
    u8 write;
} Instruction;

void instruction_fmt(FILE* stream, Instruction* i);

#endif
