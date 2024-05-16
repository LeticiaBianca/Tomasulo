#include <stdio.h>

#include "instruction.h"

static const char* _inst_names[] = {
	[Add] 		= "ADD",
	[Sub] 		= "SUB",
	[Mul] 		= "MUL",
	[Div] 		= "DIV",
	[Load] 		= "LOAD",
	[Store] 	= "STORE",
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


