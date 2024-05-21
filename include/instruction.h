#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdio.h>
#include <stdlib.h>

	#define SIZE_STR	30
	#define SIZE_TIME	5


	#define LOAD		0	
	#define ADD     	1	
	#define MULT	   	2	
	#define BRANCH	   	3	
	#define COMPARISON  4	
	#define LOGICAL	   	5	

	typedef struct Instruction {
	
		int		reorder_buffer_position;	
		int 	type;		 				
		int		issued; 					
		char*	full_instruction;			
		char**	splitted_instruction;		
		float*	time;						
	} Instruction;

#endif


extern Instruction* instructionsInitializer(char*);
extern char** readInstructions(char*, int);
extern int numberOfLines(char*);
extern char** splitInstruction(char*);
extern Instruction* cloneInstruction (Instruction* instruction_origin);
extern int findNumberOfJumps( Instruction*, int, char*, int );
extern void printInstructions(Instruction*, char*);
extern void insertTime (char* full_instruction, Instruction *instructions, int position, int amount_instructions, float seconds);
extern void printTimeInstructions(Instruction *instructions, int amount_instructions);
