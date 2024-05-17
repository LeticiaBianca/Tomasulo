#include <stdio.h>

#include "instruction.h"

static const char* _inst_names[] = {
    [Add]   = "add",
    [Sub]   = "sub",
    [Mul]   = "mul",
    [Div]   = "div",
    [Load]  = "load",
    [Store] = "store",
};

void instruction_fmt(FILE* stream, Instruction* i)
{
    fprintf(stream, "{ ");
    fprintf(stream, "kind: %s, ", _inst_names[i->type]);
    fprintf(stream, "emit: %u, ", i->emit);
    fprintf(stream, "start: %u, ", i->start);
    fprintf(stream, "exec: %u, ", i->exec);
    fprintf(stream, "write: %u ", i->write);
    fprintf(stream, "}\n");
}
