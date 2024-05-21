#include "reorder_buffer.h"
#include "instruction.h"
#include "register.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


Register_status* registerStatusInitializer(){
	Register_status* rStatus = (Register_status*) malloc( sizeof(Register_status) );
	
	for( int i = 0; i < MAX_REGISTERS; i++ ){
		char* f = (char*) malloc( 4 * sizeof(char) );
		char* F = (char*) malloc( 4 * sizeof(char) );
		F[0] = 'R';
		F[1] = '\0';
		sprintf(f, "%d", i);
		strcat(F, f);
		strcat(F, "\0");

		rStatus->column[i].field = F;
		rStatus->column[i].reorder_entry = -1;
		rStatus->column[i].register_busy = NOT_BUSY;
	}

	return rStatus;
}

int findPosRegister(char* field, Register_status* register_status){
	int pos = -1;

	for( int i = 0; i < MAX_REGISTERS; i++ ){
		if(strcmp(field, register_status->column[i].field) == 0){
			pos = i;
			i = MAX_REGISTERS;
		}
	}

	return pos;
}

int getReorderPosition(char* field, Register_status* register_status) {
	int reorder_position = -1;
	int position_register = findPosRegister(field, register_status);

	if ( position_register != -1 && register_status->column[position_register].register_busy == BUSY) {
		reorder_position = register_status->column[position_register].reorder_entry;
	}

	return reorder_position;
}

int insertRegisterStatus(char* fieldRegister, int entryReorderBuffer, Register_status* register_status){
	int position_register_status = -1,
	    controller		     = -1;

	if ( entryReorderBuffer == -1 ) {
		printf("ERROR: Invalid position reorder buffer: %d.\n", entryReorderBuffer);
		return controller;
	}

	position_register_status = findPosRegister(fieldRegister, register_status);

	if ( position_register_status == -1 ) {
		printf("ERROR: Not found register field: %s.\n", fieldRegister);
		return controller;
	}

	register_status->column[position_register_status].reorder_entry = entryReorderBuffer;
	register_status->column[position_register_status].register_busy = BUSY;

	return position_register_status;
}	
		


void printRegisterStatus(Register_status* register_status){
	int i = 0;
	int aux = 0;
	printf("\n----------Register Status----------\n");

	while (i < MAX_REGISTERS) {
		aux = i;
		printf("\n  Field:\t");
		for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
			printf("%s\t", register_status->column[aux].field);
		}
		printf("\n  Reorder#:\t");
		for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
			if (register_status->column[aux].reorder_entry == -1) {
				printf("-\t");
			} else {
				printf("%d\t", register_status->column[aux].reorder_entry);
			}
		}
		printf("\n  Busy:\t\t");
		for (aux = i; aux < (i + 8) && i < MAX_REGISTERS; aux++) {
			printf("%s\t", ( register_status->column[aux].register_busy == NOT_BUSY ) ? "No\0" : "Yes\0");
		}
		printf("\n");
		i = i + 8;
	}
}

int freeRegister(char * fieldRegister, Register_status* register_status) {
	int posRegisterStatus = findPosRegister(fieldRegister, register_status);

	if ( posRegisterStatus == -1 ) {
		printf("ERROR: Not found register field: %s.\n", fieldRegister);
	} else {
		register_status->column[posRegisterStatus].reorder_entry = -1;
		register_status->column[posRegisterStatus].register_busy = NOT_BUSY;
	}

	return posRegisterStatus;
}

char* registerNameOrPositionRB (Register_status* register_status, char* name_register, int position_instruction_rb) {
	char* result = (char*) malloc( SIZE_STR * sizeof(char) );
	int reorder_position_register = getReorderPosition(name_register, register_status);
	char* position = (char*) malloc( 2 * sizeof(char) );
	position[0] = '\0';

	if (reorder_position_register != -1 && reorder_position_register != position_instruction_rb) {
		strcpy(result, "#");
		sprintf(position, "%d", reorder_position_register);
		strcat(result, position);
		strcat(result, "\0");

	} else {
		strcpy(result, name_register);
	}

	return result;
}

char* calculateResult(Instruction* instruction, Register_status* register_status) {
	char* result = (char*) malloc( SIZE_STR * sizeof(char) );
	strcpy(result, "");
	
	if(instruction->type == ADD) {
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[2], instruction->reorder_buffer_position));
		if(strcmp(instruction->splitted_instruction[0], "ADD") == 0) {
			strcat(result, " + ");
		} else {
			strcat(result, " - ");
		}
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));

	} else if(instruction->type == LOAD) {
		if(strcmp(instruction->splitted_instruction[0], "LW") == 0) {
			strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[1], instruction->reorder_buffer_position));
			strcat(result, " = Mem[");
			strcat(result, instruction->splitted_instruction[2]);
			strcat(result, " + Regs[");
			strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));
			strcat(result, "]]");
		} else {
			strcat(result, "Mem[");
			strcat(result, instruction->splitted_instruction[2]);
			strcat(result, " + Regs[");
			strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));
			strcat(result, "]] = ");
			strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[1], instruction->reorder_buffer_position));
		}

	} else if(instruction->type == MULT) {
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[2], instruction->reorder_buffer_position));
		if(strcmp(instruction->splitted_instruction[0], "MUL") == 0) {
			strcat(result, " * ");
		} else if(strcmp(instruction->splitted_instruction[0], "DIV") == 0){
			strcat(result, " / ");
		} else {
			strcat(result, " % ");
		}
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));

	} else if(instruction->type == BRANCH) {
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[1], instruction->reorder_buffer_position));
		if(strcmp(instruction->splitted_instruction[0], "BEQ") == 0) {
			strcat(result, " == ");
		} else {
			strcat(result, " != ");
		}
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[2], instruction->reorder_buffer_position));

	} else if(instruction->type == COMPARISON) {
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[2], instruction->reorder_buffer_position));
		strcat(result, " < ");
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));

	} else if(instruction->type == LOGICAL) {
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[2], instruction->reorder_buffer_position));
		if(strcmp(instruction->splitted_instruction[0], "AND") == 0) {
			strcat(result, " && ");
		} else if(strcmp(instruction->splitted_instruction[0], "OR") == 0) {
			strcat(result, " || ");
		} else if(strcmp(instruction->splitted_instruction[0], "XOR") == 0) {
			strcat(result, " ^ ");
		} else if(strcmp(instruction->splitted_instruction[0], "SLL") == 0) {
			strcat(result, " << ");
		} else {
			strcat(result, " >> ");
		}
		strcat(result, registerNameOrPositionRB(register_status, instruction->splitted_instruction[3], instruction->reorder_buffer_position));
	}

	return result;
}