#ifndef REORDER_BUFFER_H_
#define REORDER_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30
#ifndef _REORDER_BUFFER_LINE_

		
		#define	NOT_BUSY	FALSE	
		#define BUSY		TRUE	
		
		#define WAITING			0	
		#define ISSUE			1	
		#define EXECUTING		2	
		#define	WRITE_RESULT		3	
		#define	COMMITED		4	
		
		typedef struct Reoder_Buffer_Line {
			
			BOOL		instruction_execution;	
			int		instruction_state;	
			
			char*		instruction_result;	
			Instruction* 	instruction;
		} Reorder_Buffer_Line;

#endif 

	#define MAX_LINES	 6			
	typedef struct Reorder_Buffer {
		int max_lines_rb_allocated;				
		int filled_lines;						
		Reorder_Buffer_Line line[MAX_LINES];	
	} Reorder_Buffer;

#endif 


extern Reorder_Buffer* reorderBufferInitializer();
extern int insertInstructionRB(Instruction*, Reorder_Buffer*);
extern void printReorderBuffer(Reorder_Buffer*);
extern void freeLineReorderBuffer(Reorder_Buffer *rb, int positionToFree);
extern Reorder_Buffer* freesReorderBuffer(Reorder_Buffer*);
