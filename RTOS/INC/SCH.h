/*
 * SCH.h
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_


#include "PORTING.h"



//Number of Tasks
#define Tasks 5


//Return Status of Function given
typedef enum{
	Error,success
}RTOS_Error_ID;






typedef struct{
	unsigned int Stack_Size;
	unsigned char Priority;
	void (*p_TaskEntry)(void);//Pointer to task function
	unsigned char AutoStart ;
	unsigned int _S_PSP_TASK;//Not for user
	unsigned int _E_PSP_TASK;//Not for user
	unsigned int* _C_PSP;//Not for user
	char TaskName[30];
	enum{
		Suspended,Running,Ready,Waiting
	}TaskState;//Not for user
	struct{
		enum{
		enable,disable
		}Blocking;
		unsigned int TimeTicks;
	}TimeWait;
}TCB_RTOS;

typedef struct {

	unsigned char* Ppayload ;
	unsigned int   PayloadSize ;
	TCB_RTOS* 	   CurrentTUser ;
	TCB_RTOS* 	   NextTUser ;
	char 		   MutexName[30]  ;

} Mutex_t;

RTOS_Error_ID RTOS_init();
RTOS_Error_ID RTOS_Create_Task(TCB_RTOS* Task);
RTOS_Error_ID RTOS_Activate_Task(TCB_RTOS* Task);
RTOS_Error_ID RTOS_Terminate_Task(TCB_RTOS* Task);
RTOS_Error_ID RTOS_Task_Wait(unsigned int TICKS,TCB_RTOS* SelfTref);
RTOS_Error_ID RTOS_Start();
RTOS_Error_ID RTOS_Acquire_Mutex(TCB_RTOS* Task,Mutex_t* Mutex );
RTOS_Error_ID RTOS_Release_Mutex(Mutex_t* Mutex);

#endif /* INC_SCH_H_ */
