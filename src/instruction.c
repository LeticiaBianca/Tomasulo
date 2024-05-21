#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20

int findNumberOfJumps( Instruction* instructions, int number_of_instructions, char* label_instruction, int position ) {
	int	resp = 0,
		i    = 0;

	for ( ; i < number_of_instructions; i++ ){
		if ( strstr( instructions[i].splitted_instruction[0], label_instruction ) != NULL ) {
			resp = i - position;
			i = number_of_instructions;
		}	
	}
	
	return resp;
}

char** splitInstruction(char *instruction){
	
	char	**splitted_string = (char**) malloc(( 5 * (sizeof(char*)) ));
	char	*delimiter = " ,():";
	char	*token;
	int	aux = 0;

	token = strtok(instruction, delimiter);
	while( token != NULL ) {
		*splitted_string = (char*) malloc( 10 * sizeof(char) );
		strcpy(*splitted_string, token); 
		splitted_string++;
		aux++;
		token = strtok(NULL, delimiter);
	}	

	*splitted_string = (char*) malloc( sizeof(char) );
	strcpy(*splitted_string, "\0");
	

	splitted_string -= aux;
	return splitted_string;
}


Instruction* cloneInstruction (Instruction* instruction_origin) {
	Instruction* instruction = (Instruction*) malloc( sizeof(Instruction) );
	
	int i = 0;
	instruction->full_instruction = (char*) malloc ( SIZE_STR * sizeof(char) );
	strcpy(instruction->full_instruction, instruction_origin->full_instruction);
	instruction->issued = instruction_origin->issued;
	instruction->reorder_buffer_position = instruction_origin->reorder_buffer_position;
	instruction->type = instruction_origin->type;

	instruction->time = (float*) calloc( SIZE_TIME, sizeof(float) );
	for(i = 0; i < SIZE_TIME; i++) {
		instruction->time[i] = instruction_origin->time[i];
	}

	instruction->splitted_instruction = (char**) malloc(( 5 * (sizeof(char*)) ));
	for (i = 0; i < 5; i++) {
		if ( instruction_origin->splitted_instruction[i] != NULL ) {
			instruction->splitted_instruction[i] = (char*) malloc( 10 * sizeof(char) );
			strcpy(instruction->splitted_instruction[i], instruction_origin->splitted_instruction[i]);
		}
	}

	return instruction;
}


int numberOfLines(char *file_path){
	char	foo[MAX_SIZE];
	int	lines = 0;
	FILE 	*file_pointer;

	/*
	 * There is no need to check if file exists since it was tested before in "readInstructions"
	 */
	file_pointer = fopen(file_path, "r");
	
	while(fgets(foo, sizeof(foo), file_pointer) != NULL) {
		lines++;		
	}

	fclose(file_pointer);
	return lines;
}

char** readInstructions(char *file_path, int number_of_lines){
	char	**file_inputs = NULL;
	FILE	*file_pointer;

	file_pointer = fopen(file_path, "r");

	if ( file_pointer != NULL ) {
		int	i, line_size = 0;
		file_inputs = (char **) malloc(number_of_lines * sizeof(char*));

		for( i = 0; i < number_of_lines; i++ ) {
			file_inputs[i] = (char *) malloc(MAX_SIZE * sizeof(char));
			fgets(file_inputs[i], MAX_SIZE, file_pointer);

			line_size = strlen(file_inputs[i]);
			file_inputs[i][line_size - 1] = '\0';
		}


		file_inputs[number_of_lines] = (char *) malloc(sizeof(char));
		strcpy(file_inputs[number_of_lines], "\0");
	}	
	fclose(file_pointer);
	return file_inputs;
}

Instruction* instructionsInitializer(char* filename){
	int	number_of_lines = 0,
		i = 0;
	number_of_lines = numberOfLines(filename);

	Instruction *instructions = (Instruction*) malloc( number_of_lines * sizeof(Instruction) );

	char	**instructionsRead = readInstructions(filename, number_of_lines);
	
	for( i = 0; i < number_of_lines; i++ ) {
		instructions[i].full_instruction = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(instructions[i].full_instruction, instructionsRead[i]);
		instructions[i].splitted_instruction = splitInstruction(instructionsRead[i]);
		instructions[i].time = (float*) calloc( SIZE_TIME, sizeof(float) );
	}	

	return instructions;
}


void printInstructions(Instruction *it, char* filePath){
	int	i,
		size = numberOfLines(filePath);
	printf("Instructions: \n");
	for ( i = 0; i < size ; i++ ){
		printf("%s\n", it[i].full_instruction);
	}
}

void insertTime (char* full_instruction, Instruction *instructions, int position, int amount_instructions, float seconds) {

	for (int i = 0; i < amount_instructions; i++) {
		if (strcmp(instructions[i].full_instruction, full_instruction) == 0 && instructions[i].time[position] == 0) {
			instructions[i].time[position] = seconds;
			i = amount_instructions;
		}
	}
}
