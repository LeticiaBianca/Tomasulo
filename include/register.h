#ifndef REGISTER_STATUS_H_

#define REGISTER_STATUS_H_

#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "reorder_buffer.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30

#ifndef REGISTER_STATUS_COLUMN_

		
		#define	NOT_BUSY	FALSE	
		#define BUSY		TRUE	
		
		typedef struct Register_status_column {
			
			BOOL register_busy;  
			
			
			char* field;     
            		int reorder_entry;  
			
		} Register_status_column;

#endif 

	
	#define MAX_REGISTERS	 32

	typedef struct Register_status {
		Register_status_column column[MAX_REGISTERS];	
	} Register_status;

#endif 

extern Register_status* registerStatusInitializer();
extern int insertRegisterStatus(char* fieldRegister, int entryReorderBuffer, Register_status* register_status);
extern void printRegisterStatus(Register_status* register_status);
extern int findPosRegister(char* field, Register_status* register_status);
extern int getReorderPosition(char* field, Register_status* register_status);
extern int freeRegister(char * fieldRegister, Register_status* register_status);
extern char* registerNameOrPositionRB (Register_status* register_status, char* name_register, int position_instruction_rb);
extern char* calculateResult(Instruction* instruction, Register_status* register_status);