#include <stdio.h>

#include "instruction.h"

static const char* _inst_names[] = {
    [Add] 	= "add",
    [Sub] 	= "sub",
    [Mul] 	= "mul",
    [Div] 	= "div",
    [Load] 	= "load",
    [Store] = "store",
};

void instruction_fmt(FILE* stream, Instruction* i)
{
    fprintf(stream,
	"{ "
	  "kind: %s, "
	  "emit: %u, "
	  "start: %u, "
	  "exec: %u, "
	  "write: %u "
	"}\n",
        _inst_names[i->kind],
        i->emit,
        i->start,
        i->exec,
        i->write);
}
