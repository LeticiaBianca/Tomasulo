#include "reorder_buffer.h"
#include "instruction.h"
#include <string.h>
#include <stdio.h>


Reorder_Buffer* reorderBufferInitializer(){
	Reorder_Buffer* rb = (Reorder_Buffer*) malloc( sizeof(Reorder_Buffer) );

	rb->max_lines_rb_allocated = 0;
	rb->filled_lines = 0;	

	return rb;
}

int insertInstructionRB(Instruction *instruction, Reorder_Buffer *rb){
	
	int	position = rb->filled_lines % MAX_LINES;

	rb->line[position].instruction_execution = BUSY;
	
	rb->line[position].instruction = cloneInstruction(instruction);

	rb->line[position].instruction_state = WAITING;

	if (rb->line[position].instruction_result == NULL) {
		rb->line[position].instruction_result = (char*) malloc( SIZE_STR * sizeof(char) );
	}
	strcpy(rb->line[position].instruction_result, "NOT CALCULATED YET\0");

	rb->line[position].instruction->reorder_buffer_position = position;

	rb->filled_lines += 1;

	if (rb->filled_lines < MAX_LINES) {
		rb->max_lines_rb_allocated = rb->filled_lines;
	} else {
		rb->max_lines_rb_allocated = MAX_LINES;
	}

	return position;
}

void printReorderBuffer(Reorder_Buffer *rb){
	int	i = 0,
		init = rb->filled_lines % rb->max_lines_rb_allocated,
		end = ((init - 1) < 0 ? (rb->max_lines_rb_allocated - 1) : (init - 1));
	printf("\n\n----------Reorder Buffer----------\n\n");
	printf("  Entry\t\tBusy\tInstruction\t\t\tState\t\tDestination\tValue\n");
	for( i = init; i != end; i = ((i + 1) % rb->max_lines_rb_allocated) ) {
		printf("    %d\t\t", i);
		printf("%s\t", ( rb->line[i].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
		printf("%s\t\t", (rb->line[i].instruction->full_instruction == NULL ? "-" : (strcmp(rb->line[i].instruction->full_instruction, "") == 0 ? "-" : rb->line[i].instruction->full_instruction)));
		if ( rb->line[i].instruction->type != BRANCH && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
		if ( (rb->line[i].instruction->full_instruction == NULL || strcmp(rb->line[i].instruction->full_instruction, "") == 0 || strcmp(rb->line[i].instruction->full_instruction, "-") == 0) && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
		if (rb->line[i].instruction_execution == NOT_BUSY && i == 5 && (rb->line[i].instruction->full_instruction == NULL || strcmp(rb->line[i].instruction->full_instruction, "") == 0 || strcmp(rb->line[i].instruction->full_instruction, "-") == 0)) {
			printf("\t");
		}
		printf("%s\t", (rb->line[i].instruction_state == WAITING ? "WAITING" : (rb->line[i].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[i].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[i].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
		if ( rb->line[i].instruction_state < 2 ) {
			printf("\t");
		}
		if (rb->line[i].instruction->type == BRANCH || (rb->line[i].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[i].instruction->splitted_instruction[0], "SW") == 0)) {
			printf("    %s\t\t", "-");
		} else {
			printf("    %s\t\t", (rb->line[i].instruction->splitted_instruction[1] == NULL ? "-" : (strcmp(rb->line[i].instruction->splitted_instruction[1], "") == 0 ? "-" : rb->line[i].instruction->splitted_instruction[1])));
		}
		printf("%s\n", (rb->line[i].instruction_result == NULL ? "-" : (strcmp(rb->line[i].instruction_result, "") == 0 ? "-" : rb->line[i].instruction_result)));
	}
	printf("    %d\t\t", end);
	printf("%s\t", ( rb->line[end].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
	printf("%s\t\t", (rb->line[end].instruction->full_instruction == NULL ? "-" : (strcmp(rb->line[end].instruction->full_instruction, "") == 0 ? "-" : rb->line[end].instruction->full_instruction)));
	if ( rb->line[i].instruction->type != BRANCH && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
	}
	if ( (rb->line[end].instruction->full_instruction == NULL || strcmp(rb->line[end].instruction->full_instruction, "") == 0) && (strlen(rb->line[i].instruction->splitted_instruction[1]) + strlen(rb->line[i].instruction->splitted_instruction[2]) + strlen(rb->line[i].instruction->splitted_instruction[3])) < 9) {
			printf("\t");
		}
	printf("%s\t", (rb->line[end].instruction_state == WAITING ? "WAITING" : (rb->line[end].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[end].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[end].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
	if ( rb->line[end].instruction_state < 2 ) {
		printf("\t");
	}
	if (rb->line[end].instruction->type == BRANCH || (rb->line[end].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[end].instruction->splitted_instruction[0], "SW") == 0)) {
		printf("    %s\t\t", "-");
	} else {
		printf("    %s\t\t", (rb->line[end].instruction->splitted_instruction[1] == NULL ? "-" : (strcmp(rb->line[end].instruction->splitted_instruction[1], "") == 0 ? "-" : rb->line[end].instruction->splitted_instruction[1])));
	}
	printf("%s\n", (rb->line[end].instruction_result == NULL ? "-" : (strcmp(rb->line[end].instruction_result, "") == 0 ? "-" : rb->line[end].instruction_result)));
}

void freeLineReorderBuffer(Reorder_Buffer *rb, int positionToFree) {
	rb->line[positionToFree].instruction_state = WAITING;
	strcpy(rb->line[positionToFree].instruction->full_instruction, ""); 
	strcpy(rb->line[positionToFree].instruction->splitted_instruction[1], "");
	strcpy(rb->line[positionToFree].instruction_result, "");
	rb->line[positionToFree].instruction_execution = NOT_BUSY;
}


Reorder_Buffer* freesReorderBuffer(Reorder_Buffer *rb) {
	int i = 0;
    if (rb == NULL)
		printf("ERROR: Invalid reorder buffer.\n");
        return (NULL);
    if (rb->max_lines_rb_allocated < 1)
    {
        printf("ERROR: Invalid maximum rows allocated.\n");
        return (rb);
    }
    for (i = 0; i < rb->max_lines_rb_allocated; i++) {
		free(rb->line[i].instruction);
		free(rb->line[i].instruction_result);
	}
		
    free(rb);     
    return (NULL);
}
