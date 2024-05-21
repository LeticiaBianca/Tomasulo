#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reorder_buffer.h"
#include "instruction.h"
#include "station.h"
#include "register.h"

#define MAX_SIZE 20


char** freeCharacterMatrix(char **matrix, int size)
{
    int i = 0;
    if (matrix == NULL)
		printf("ERROR: Invalid matrix.\n");
        return (NULL);
    if (size < 1)
    {
        printf("ERROR: Invalid size parameter.\n");
        return (matrix);
    }
    for (i = 0; i < size; i++)
        free(matrix[i]); 	
    free(matrix);   
    return (NULL);
}

int decoder(char* operation){
	int	resp = -1;

	if ( strcmp(operation, "LW") == 0 || strcmp(operation, "SW") == 0 )  {
		resp = LOAD;
	} else if ( strcmp(operation, "ADD") == 0 || strcmp(operation, "SUB") == 0 ) {
		resp = ADD;
	} else if ( strcmp(operation, "MUL") == 0 || strcmp(operation, "DIV") == 0 || strcmp(operation, "REM") == 0 ) {
		resp = MULT;
	} else if ( strcmp(operation, "BNE") == 0 || strcmp(operation, "BEQ") == 0 ) {
		resp = BRANCH;
	} else if ( strcmp(operation, "SLT") == 0 || strcmp(operation, "SLTI") == 0 ) {
		resp = COMPARISON;
	} else if ( strcmp(operation, "OR") == 0 || strcmp(operation, "AND") == 0 || strcmp(operation, "XOR") == 0 || strcmp(operation, "SLL") == 0 || strcmp(operation, "SRL") == 0 || strcmp(operation, "SRA") == 0 ) {
		resp = LOGICAL;
	}

	return resp;
}

void writeTablesToFile (char* fileName, char *writing_mode, Reorder_Buffer *rb, Reservation_Station *rs, Register_status *registerRename)
{
    FILE *arquivo = fopen(fileName, writing_mode);

	if (arquivo != NULL) {
		int	i = 0,
		init = rb->filled_lines % rb->max_lines_rb_allocated,
		end = ((init - 1) < 0 ? (rb->max_lines_rb_allocated - 1) : (init - 1));
		fprintf(arquivo, "%s", "----------Reorder Buffer----------\n\n");
		fprintf(arquivo, "%s",  "Entry\tBusy\tInstruction\t\t\t\t\t\tState\t\t\t\t\tDestination\t\tValue\n");
		
		for( i = init; i != end; i = ((i + 1) % rb->max_lines_rb_allocated) ) {
			fprintf(arquivo, "  %d\t\t", i);
			fprintf(arquivo, "%s\t\t", ( rb->line[i].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
			fprintf(arquivo, "%s\t\t", rb->line[i].instruction->full_instruction);
			if (rb->line[i].instruction->type == LOAD || strcmp(rb->line[i].instruction->splitted_instruction[0], "OR") == 0 ) {
				fprintf(arquivo, "\t");
			}
			if ( rb->line[i].instruction->type != BRANCH ) {
				fprintf(arquivo, "\t\t");
			}
			fprintf(arquivo, "%s\t", (rb->line[i].instruction_state == WAITING ? "WAITING" : (rb->line[i].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[i].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[i].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
			if (rb->line[i].instruction_state == ISSUE) {
				fprintf(arquivo, "\t\t\t\t");
			} else if (rb->line[i].instruction_state == WAITING) {
				fprintf(arquivo, "\t\t\t");
			} else if (rb->line[i].instruction_state == EXECUTING || rb->line[i].instruction_state == COMMITED) {
				fprintf(arquivo, "\t\t");
			}
			if (rb->line[i].instruction->type == BRANCH || (rb->line[i].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[i].instruction->splitted_instruction[0], "SW") == 0)) {
				fprintf(arquivo, "    -\t\t\t\t");
			} else {
				fprintf(arquivo, "    %s\t\t\t\t", rb->line[i].instruction->splitted_instruction[1]);			
			}
			fprintf(arquivo, "%s\n", rb->line[i].instruction_result);
		}
		
		fprintf(arquivo, "  %d\t\t", end);
		fprintf(arquivo, "%s\t\t", ( rb->line[end].instruction_execution == NOT_BUSY ) ? "No\0" : "Yes\0");
		fprintf(arquivo, "%s\t\t", rb->line[end].instruction->full_instruction);
		
		if (rb->line[end].instruction->type == LOAD || strcmp(rb->line[end].instruction->splitted_instruction[0], "OR") == 0 ) {
			fprintf(arquivo, "\t");
		}
		if ( rb->line[end].instruction->type != BRANCH ) {
			fprintf(arquivo, "\t\t");
		}	
		fprintf(arquivo, "%s\t", (rb->line[end].instruction_state == WAITING ? "WAITING" : (rb->line[end].instruction_state == EXECUTING ? "EXECUTING" : (rb->line[end].instruction_state == WRITE_RESULT ? "WRITE_RESULT" : (rb->line[end].instruction_state == ISSUE ? "ISSUE" : "COMMITED")))));
		if (rb->line[end].instruction_state == ISSUE) {
			fprintf(arquivo, "\t\t\t\t");
		} else if (rb->line[end].instruction_state == WAITING) {
			fprintf(arquivo, "\t\t\t");
		} else if (rb->line[end].instruction_state == EXECUTING || rb->line[end].instruction_state == COMMITED) {
			fprintf(arquivo, "\t\t");
		}
		if (rb->line[end].instruction->type == BRANCH || (rb->line[end].instruction->splitted_instruction[0] != NULL && strcmp(rb->line[end].instruction->splitted_instruction[0], "SW") == 0)) {
			fprintf(arquivo, "    -\t\t\t\t");
		} else {
			fprintf(arquivo, "    %s\t\t\t\t", rb->line[end].instruction->splitted_instruction[1]);
		}
		fprintf(arquivo, "%s\n", rb->line[end].instruction_result);
		

		fprintf(arquivo, "%s", "\n\n----------Reservation Station----------\n\n");
		fprintf(arquivo, "%s", "Name\t\t\t\t\tBusy\t\tOp\t\t\tVj\t\t\t\tVk\t\t\t\tQj\t\tQk\t\tDest\t\tAddress\n");
		for( i = 0; i < MAX_LINES_RS; i++ ) {
			fprintf(arquivo, "%s\t\t", rs->line[i].name);
			if(i < 7) {
				fprintf(arquivo, "\t\t\t");
			} else if (i == 7) {
				fprintf(arquivo, "\t\t");
			} else if (i > 8) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, "%s\t\t\t", ( rs->line[i].reservation_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
			fprintf(arquivo, "%s\t\t", (rs->line[i].instruction_op == NULL ? "-" : (strcmp(rs->line[i].instruction_op, "") == 0 ? "-" : rs->line[i].instruction_op) ));
			if (rs->line[i].instruction_op == NULL || strcmp(rs->line[i].instruction_op, "") == 0) {
				fprintf(arquivo, "\t\t");
			} else if (strcmp(rs->line[i].instruction_op, "SLTI") != 0) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, "%s\t\t\t\t", (rs->line[i].value_register_read_Vj == NULL ? "-" : (strcmp(rs->line[i].value_register_read_Vj, "") == 0 ? "-" : (strcmp(rs->line[i].instruction_op, "LW") == 0 ? "-" : rs->line[i].value_register_read_Vj))));
			if (rs->line[i].value_register_read_Vj == NULL || strcmp(rs->line[i].value_register_read_Vj, "") == 0 || strcmp(rs->line[i].instruction_op, "LW") == 0) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, "%s\t\t\t\t", (rs->line[i].value_register_read_Vk == NULL ? "-" : (strcmp(rs->line[i].value_register_read_Vk, "") == 0 ? "-" : rs->line[i].value_register_read_Vk)));
			if (rs->line[i].value_register_read_Vk == NULL || strcmp(rs->line[i].value_register_read_Vk, "") == 0) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, "%d\t\t", rs->line[i].information_dependency_Qj);
			if (rs->line[i].information_dependency_Qj != -1) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, "%d\t\t", rs->line[i].information_dependency_Qk);
			if (rs->line[i].information_dependency_Qk != -1) {
				fprintf(arquivo, "\t");
			}
			fprintf(arquivo, " %d\t\t\t", rs->line[i].position_destination_rb);
			fprintf(arquivo, "%s\n", (rs->line[i].memory_address == NULL ? "-" : (strcmp(rs->line[i].memory_address, "") == 0 ? "-" : rs->line[i].memory_address)));
		}
		i = 0;
		int aux = 0;
		fprintf(arquivo, "\n----------Register Status----------\n");

		while (i < MAX_REGISTERS) {
			aux = i;
			fprintf(arquivo, "\nField:\t\t\t");
			for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
				fprintf(arquivo, "%s\t\t\t", registerRename->column[aux].field);
			}
			fprintf(arquivo, "\nReorder#:\t\t");
			for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
				fprintf(arquivo, "%d\t\t\t", registerRename->column[aux].reorder_entry);
				if (registerRename->column[aux].reorder_entry != -1) {
					fprintf(arquivo, "\t");
				}
			}
			fprintf(arquivo, "\nBusy:\t\t\t\t");
			for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
				fprintf(arquivo, "%s\t\t\t", ( registerRename->column[aux].register_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
			}
			fprintf(arquivo, "\n");
			i = i + 8;
		}

		fprintf(arquivo, "%s\n\n", "_____________________________________________________________________________________________________________");
	}

    fclose(arquivo);
}

int noDependencies(int qj, int qk){
	return ((qj == -1) && (qk == -1));
}

void initializer(char* filename){

	printf("----------Tomasulo---------\n");
	Reorder_Buffer *rb = reorderBufferInitializer();
	Instruction *instructions = instructionsInitializer(filename);
	Reservation_Station *rs = reservationStationInitializer();
	Register_status *registerRename = registerStatusInitializer();

	printInstructions(instructions, filename);

	int number_of_instructions = numberOfLines(filename);
	int init = rb->filled_lines % MAX_LINES,
	    end  = ((init - 1) % MAX_LINES < 0) ? MAX_LINES - 1 : (init - 1);
	int i, j, aux, 
	    position = 0;
	int old_position[MAX_LINES];
	int *position_run = (int*) calloc( MAX_LINES_RS, sizeof(int) );
	float elapsed_milliseconds = 0;

	FILE* table = fopen("tables.txt", "wt");
	fclose(table);

	for (i = 0; i < MAX_LINES_RS; i++) {
		position_run[i] = -1;
	}
	
	while ( rb->line[end].instruction_state != COMMITED || position != number_of_instructions ) {
		init = rb->filled_lines % (rb->max_lines_rb_allocated == 0 ? 
				MAX_LINES :
			       	rb->max_lines_rb_allocated);

		end = ( (init - 1) % (rb->max_lines_rb_allocated == 0 ? MAX_LINES : rb->max_lines_rb_allocated) < 0 ) ? 
			((rb->max_lines_rb_allocated == 0 ? MAX_LINES : rb->max_lines_rb_allocated) - 1) : 
			(init - 1);

		for ( i = init ; i != end && position < number_of_instructions; i = (i + 1) % MAX_LINES ) {
			
			if ( strstr(instructions[position].full_instruction, ":") != NULL ) {
				position ++;
			}
			
			if ( position < number_of_instructions && rb->line[i].instruction_execution != BUSY ) {
				if (rb->line[i].instruction_state == COMMITED && dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
										freeRegister(rb->line[i].instruction->splitted_instruction[1], registerRename);
				}
				instructions[position].time[WAITING] = elapsed_milliseconds;
				
				int position_reorder = insertInstructionRB( &instructions[position], rb );
				rb->line[i].instruction->reorder_buffer_position = position_reorder;
				rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] );

				if ( rb->line[i].instruction->type == BRANCH ) {
					old_position[i] = position;
					position += findNumberOfJumps( instructions,
							               number_of_instructions,
							               rb->line[i].instruction->splitted_instruction[3],
								       old_position[i] );
				} 
				position ++;
			}
		}	
		if ( strstr(instructions[position].full_instruction, ":") != NULL ) {
			position ++;
		}
		if ( position < number_of_instructions && rb->line[i].instruction_execution != BUSY ) {
			if (rb->line[i].instruction_state == COMMITED && dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
								freeRegister(rb->line[i].instruction->splitted_instruction[1], registerRename);
			}
			instructions[position].time[WAITING] = elapsed_milliseconds;

			int position_reorder = insertInstructionRB( &instructions[position], rb );
			rb->line[i].instruction->reorder_buffer_position = position_reorder;
			rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] ); 
			
			if ( rb->line[i].instruction->type == BRANCH ) {
				old_position[i] = position;
				position += findNumberOfJumps( instructions,
						               number_of_instructions,
					        	       rb->line[i].instruction->splitted_instruction[3],
							       old_position[i] );
			} 
			position ++;
		
		}
		printReorderBuffer(rb);
		printReservationStation(rs);
		printRegisterStatus(registerRename);

		init = rb->filled_lines % rb->max_lines_rb_allocated;
		end = ( ((init - 1) < 0 ? 
					(rb->max_lines_rb_allocated - 1) :
				       	(init - 1)) % rb->max_lines_rb_allocated);
		for ( i = init; i != end; i = (i + 1) % rb->max_lines_rb_allocated ) {
			if ( rb->line[i].instruction_state == WAITING ) { 
				int logical = insertInstructionRS(rb->line[i].instruction, rs, rb);
				if (logical != -1 ) {
					rb->line[i].instruction_state = ISSUE;
					insertTime(rb->line[i].instruction->full_instruction,
						   instructions,
						   ISSUE,
						   number_of_instructions,
						   elapsed_milliseconds);
				}
			}
		}

		if ( rb->line[i].instruction_state == WAITING ) {
			int logical = 0;
			if ( (logical = insertInstructionRS(rb->line[i].instruction, rs, rb)) != -1){
				rb->line[i].instruction_state = ISSUE;
				insertTime(rb->line[i].instruction->full_instruction,
					   instructions,
					   ISSUE,
					   number_of_instructions,
					   elapsed_milliseconds);
			}		
		}
		writeTablesToFile("tables.txt", "a", rb, rs, registerRename);
		printReorderBuffer(rb);
		printReservationStation(rs);
		printRegisterStatus(registerRename);

		aux = 0;

		for ( i = 0; i < MAX_LINES_RS; i++ ){
			if ( rs->line[i].reservation_busy == NOT_BUSY ) continue; 

			if ( (noDependencies( rs->line[i].information_dependency_Qj, rs->line[i].information_dependency_Qk ) == 1) && 
				rb->line[rs->line[i].position_destination_rb].instruction_state == ISSUE) {
				int inst_position = rs->line[i].position_destination_rb;
				rb->line[inst_position].instruction_state = EXECUTING;

				insertTime(rb->line[inst_position].instruction->full_instruction,
					   instructions,
					   EXECUTING,
					   number_of_instructions,
					   elapsed_milliseconds);
				
				position_run[aux] = i;
				aux++;
			}
		}
		writeTablesToFile("tables.txt", "a", rb, rs, registerRename);
		printReorderBuffer(rb);
		printReservationStation(rs);
		printRegisterStatus(registerRename);
		
		fflush(stdin);
		getchar();
		system("cls || clear");


		init = rb->filled_lines % rb->max_lines_rb_allocated;
		end = ( ((init - 1) < 0 ?
				       	(rb->max_lines_rb_allocated - 1) :
				       	(init - 1)) % rb->max_lines_rb_allocated);

		for ( i = init; i != end; i = (i + 1) % rb->max_lines_rb_allocated ) {
			if ( rb->line[i].instruction_state == EXECUTING ) { 
				rb->line[i].instruction_state = WRITE_RESULT;
				insertTime(rb->line[i].instruction->full_instruction,
					   instructions,
					   WRITE_RESULT,
					   number_of_instructions,
					   elapsed_milliseconds);
				
				strcpy( rb->line[i].instruction_result, calculateResult(rb->line[i].instruction, registerRename) );
			}
		}
		if ( rb->line[i].instruction_state == EXECUTING ) {
			rb->line[i].instruction_state = WRITE_RESULT;
			insertTime(rb->line[i].instruction->full_instruction,
				   instructions,
				   WRITE_RESULT,
				   number_of_instructions,
				   elapsed_milliseconds);

			strcpy( rb->line[i].instruction_result, calculateResult(rb->line[i].instruction, registerRename) );
		}

		aux = 0;
		while (position_run[aux] != -1){
			i = position_run[aux];
			int inst_position = rs->line[i].position_destination_rb;

			for ( j = 0; j < MAX_LINES_RS; j++ ){
				if ( rs->line[j].reservation_busy == NOT_BUSY ) continue;

				if ( rs->line[j].information_dependency_Qj == inst_position ) {
					strcpy( rs->line[j].value_register_read_Vj, rb->line[inst_position].instruction->splitted_instruction[1] );
					rs->line[j].information_dependency_Qj = -1;
				}
				if ( rs->line[j].information_dependency_Qk == inst_position ) {
					strcpy( rs->line[j].value_register_read_Vk, rb->line[inst_position].instruction->splitted_instruction[1] );
					rs->line[j].information_dependency_Qk = -1;	
				}
			}
			
			if ( rb->line[inst_position].instruction->type == BRANCH ) {
				if ( strcmp( rb->line[inst_position].instruction->splitted_instruction[4], "#0" ) == 0 ) {
					position = old_position[inst_position];
					position++;
					
					init = rb->filled_lines % rb->max_lines_rb_allocated;
					end = ( ((init - 1) < 0 ?
									(rb->max_lines_rb_allocated - 1) :
									(init - 1)) % rb->max_lines_rb_allocated);
					int k = ((inst_position + 1) >= rb->max_lines_rb_allocated ? ((inst_position + 1) % rb->max_lines_rb_allocated) : (inst_position + 1));
					for ( ; k != init; k = (k + 1) % rb->max_lines_rb_allocated ) {
						if ( rb->line[k].instruction_state == ISSUE || rb->line[k].instruction_state == EXECUTING) { 
							clearLineRS(rs, findLineRSAccordingPositionRB(rs, k));
						}
						if ( rb->line[k].instruction_state == WRITE_RESULT) { 
							freeRegister(rb->line[k].instruction->splitted_instruction[1], registerRename);
						}
						freeLineReorderBuffer(rb, k);
					}
				}
			}

			clearLineRS(rs, i);
			aux++;
		}

		for (i = 0; i < MAX_LINES_RS; i++) {
			position_run[i] = -1;
		}
		
		writeTablesToFile("tables.txt", "a", rb, rs, registerRename);
		printReorderBuffer(rb);
		printReservationStation(rs);
		printRegisterStatus(registerRename);
		fflush(stdin);
		getchar();
		system("cls || clear");
		

		init = rb->filled_lines % rb->max_lines_rb_allocated;
		end = ( ((init - 1) < 0 ? (rb->max_lines_rb_allocated - 1) : (init - 1)) % rb->max_lines_rb_allocated);
		int controll_commit = 0;
		
		for ( i = init; i != end && controll_commit == 0; i = (i + 1) % rb->max_lines_rb_allocated ) {
			if ( rb->line[i].instruction_state == WRITE_RESULT && rb->line[i].instruction_execution == BUSY) {
				rb->line[i].instruction_state = COMMITED;
				if (dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
					insertRegisterStatus( rb->line[i].instruction->splitted_instruction[1], i, registerRename);
				}
				rb->line[i].instruction_execution = NOT_BUSY;
				insertTime(rb->line[i].instruction->full_instruction, instructions, COMMITED, number_of_instructions, elapsed_milliseconds);
			} else if ( rb->line[i].instruction_state != COMMITED ) {
				controll_commit = 1;       
			}
		}
		if (controll_commit == 0 && rb->line[end].instruction_state == WRITE_RESULT && rb->line[i].instruction_execution == BUSY) {
			rb->line[end].instruction_state = COMMITED;
			if (dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
				insertRegisterStatus( rb->line[i].instruction->splitted_instruction[1], i, registerRename);
			}
			rb->line[end].instruction_execution = NOT_BUSY;
			insertTime(rb->line[end].instruction->full_instruction, instructions, COMMITED, number_of_instructions, elapsed_milliseconds);
		}
	
		writeTablesToFile("tables.txt", "a", rb, rs, registerRename);
		printReorderBuffer(rb);
		printReservationStation(rs);
		printRegisterStatus(registerRename);
	}
	printf("----------Successfull!!----------\n");
}

int main(void){
	initializer("examples/war-waw-hazard.txt");
	return 0;
}
