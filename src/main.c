#include <stdio.h>

#include "cli.h"

int main(int argc, char** argv)
{
	Cli cli = {0};

	cli_parse(&cli, argc, argv);

	return 0;
}

