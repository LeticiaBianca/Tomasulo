#include <argp.h>

#include "cli.h"

PRIVATE const char* doc = "Tomasulo's algorithm simulator";

PRIVATE const char* args_doc = "PATH";

PRIVATE struct argp_option opts[] = {
    {"verbose", 'v', 0, false, "Produce verbose output", 0},
    {0},
};

PRIVATE error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    Cli* cli = state->input;

    switch (key) {
    case 'v':
        cli->verbose = true;
        break;
    case ARGP_KEY_ARG: {
        if (state->arg_num > 1)
            /// Too many arguments.
            argp_usage(state);
        cli->path = arg;
        break;
    }
    case ARGP_KEY_END: {
        if (state->arg_num < 1)
            /// Not enough arguments.
            argp_usage(state);
        break;
    }
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

void cli_parse(Cli* cli, int argc, char** argv)
{
    struct argp parser = {
        opts,
        parse_opt,
        args_doc,
        doc,
        0,
        0,
        0,
    };

    argp_parse(&parser, argc, argv, 0, 0, cli);
}
