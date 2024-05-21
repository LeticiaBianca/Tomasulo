
#ifndef RESERVATION_STATION_H_
#define RESERVATION_STATION_H_

#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"
#include "reorder_buffer.h"

#define BOOL	 	int
#define TRUE	 	1
#define FALSE	 	0 
#define SIZE_STR	30

#ifndef RESERVATION_STATION_LINE_

		
		#define	NOT_BUSY	FALSE	
		#define BUSY		TRUE	
		
		#define LOAD		0	
		#define ADD     	1	
		#define MULT	    2	
		
		typedef struct Reservation_Station_Line {
			
			BOOL reservation_busy;	
			
			char* name;     
			int type;      
			char* instruction_op;		
			char* value_register_read_Vj;	
			char* value_register_read_Vk;	
			int information_dependency_Qj;    
			int information_dependency_Qk;    
			int position_destination_rb;  
			char* memory_address;  
		} Reservation_Station_Line;

#endif 

	
	#define MAX_LINES_RS	 12	
	

	typedef struct Reservation_Station {
		Reservation_Station_Line line[MAX_LINES_RS];
	} Reservation_Station;

#endif


extern Reservation_Station* reservationStationInitializer();
extern int insertInstructionRS(Instruction *instruction, Reservation_Station *reservationStation, Reorder_Buffer *rb);
extern int findLineRSAccordingPositionRB (Reservation_Station *rs, int positionRB);
extern int dontDoWrite(char* nameInstruction);
extern int warDependencyIdentifier(char*, int, Reorder_Buffer*);
extern void printReservationStation(Reservation_Station*);
extern void clearLineRS (Reservation_Station *reservationStation, int positionRS);

