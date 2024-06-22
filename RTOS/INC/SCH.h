/*
 * SCH.h
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_


#include "PORTING.h"


#define Tasks 100 //Number of Tasks Used

//Return Tasks Current State
typedef enum{
	Suspended,Running,Ready,Waiting
}Task_State;

//Return Status of Function given
typedef enum{
	Error,success
}RTOS_Error_ID;


RTOS_Error_ID RTOS_init();



typedef struct{

	unsigned int Stack_Size;
	unsigned char Priority;
	void (*p_TaskEntry)(void);//Pointer to task function
	unsigned int _S_PSP_TASK;//Not for user
	unsigned int _E_PSP_TASK;//Not for user
	unsigned int* _C_PSP;//Not for user
	char TaskName[30];
	Task_State TaskState;//Not for user
	struct{enum{enable,disable}Blocking; unsigned int TimeTicks;}TimeWait;

}TCB_RTOS;

RTOS_Error_ID RTOS_Create_Task(TCB_RTOS* Task);
#endif /* INC_SCH_H_ */
