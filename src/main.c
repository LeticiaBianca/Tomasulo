#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "instruction.h"

void read_file(FILE* input, Instruction* queue);

int main(int argc, char** argv)
{
    Cli cli = {0};
    Instruction instQueue[100] = {0};

    cli_parse(&cli, argc, argv);
    FILE *input = fopen(cli.path, "r");
    read_file(input, instQueue);
    fclose(input);

    printf("--------------------------------------------------------\n");
    for(int i = 0; i < 10; i++) {
        instruction_fmt(stdout, &instQueue[i]);
    }

    return 0;
}

void read_file(FILE* input, Instruction* queue) {
    char buffer[20];
    int count = 0;

    while(fgets(buffer, sizeof buffer, input)!= NULL) {
        char* instType = strtok(buffer, " ");

        if(strcmp(instType,"sr") == 0) queue[count].type = Store;
        else if(strcmp(instType,"ld") == 0) queue[count].type = Load;
        else if(strcmp(instType,"add") == 0) queue[count].type = Add;
        else if(strcmp(instType,"sub") == 0) queue[count].type = Sub;
        else if(strcmp(instType,"mul") == 0) queue[count].type = Mul;
        else if(strcmp(instType,"div") == 0) queue[count].type = Div;
        // printf("queue[i] type: %d\n", queue[i].type);

        if(queue[count].type == Store || queue[count].type == Load) {
            char* reg = strtok(NULL, ",()");
            queue[count].r1 = reg;
            reg = strtok(NULL, ",");
            queue[count].rdest = reg;
            // printf("r1: %s\n rdest: %s\n\n", queue[i].r1, queue[i].rdest);
        }
        else {
            char* reg = strtok(NULL, ",()");
            queue[count].r1 = reg;
            reg = strtok(NULL, ",");
            queue[count].r2 = reg;
            reg = strtok(NULL, ",");
            queue[count].rdest = reg;
            // printf("r1: %s\n r2: %s\n rdest: %s\n\n", queue[i].r1, queue[i].r2, queue[i].rdest);
        }
        count++;
    }
}