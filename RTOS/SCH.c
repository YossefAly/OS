/*
 * SCH.c
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */
#include "string.h"
#include "SCH.h"
#include "RTOSFIFO.h"

struct{

	TCB_RTOS* OSTasks[Tasks];//SCH Table
	unsigned int _S_MSP_TASK;
	unsigned int _E_MSP_TASK;
	unsigned int PSP_Task_Locator;
	unsigned int NumberOfActiveTasks;
	TCB_RTOS* CurrentTask;
	TCB_RTOS* NextTask;
	enum{OS_Suspend,OS_run}OS_mode;

}OS_Control;

Queue_t Ready_Queue;
TCB_RTOS* Ready_Queue_FIFO[Tasks];


/*Excute specific  OS service*/
void SVC_Handler_C(int * SFargs){


	unsigned int __stacked_PC = SFargs[6];

	unsigned char SvcNumber = *((unsigned char*)((unsigned char*)(__stacked_PC)-2));

	switch (SvcNumber){

	case 0://Activate Task

		break;

	case 1://Terminate Task

		break;

	case 2:

		break;

	case 3:

		break;

	default:

		break;

	}
}

void OS_SVC_SET(int SVC_ID){

	switch(SVC_ID){

	case 1:

		 __asm("SVC 0x01");
		break;
	case 2:
		 __asm("SVC 0x02");
		break;
	case 3:
		 __asm("SVC 0x03");
		break;
	default: break;
	}

}


void PendSV_Handler(){

}

void RTOS_Create_MStack(){

	OS_Control._S_MSP_TASK = 0x20005000;
	OS_Control._E_MSP_TASK = OS_Control._S_MSP_TASK - 5000;
	OS_Control.PSP_Task_Locator = (OS_Control._E_MSP_TASK - 8);
	unsigned int Value = OS_Control._E_MSP_TASK;
	#if (Value <  _eheap)
	#error "Stack Size exceeds limits"
	#endif
}

void IdleTask(){

	__asm(	"IDLE: \n\t"\
			"NOP \n\t"\
			"B IDLE");

}


RTOS_Error_ID RTOS_init(){
	RTOS_Error_ID Status;

	//Update Os mode
	OS_Control.OS_mode=OS_Suspend;


	//Specify main stack for OS
	RTOS_Create_MStack();


	//Create OS ready Queue
	if(Queue_init(&Ready_Queue,Ready_Queue_FIFO, Tasks)==Queue_Work)Status=success;

	//Create Idle Task
	strcpy(Ready_Queue_FIFO[0]->TaskName,"IdleTask");//Idle Task is always Task no.1 in Queue (0 index)
	Ready_Queue_FIFO[0]->Priority = 255;
	Ready_Queue_FIFO[0]->p_TaskEntry = IdleTask;
	Ready_Queue_FIFO[0]->Stack_Size = 100 ;

	RTOS_Create_Task(Ready_Queue_FIFO[0]);

	return Status;
}

void RTOS_Create_Stack(TCB_RTOS* Task){
	/***Task Frame***
		 **R0**
		 **R1**
		 **R2**
		 **R3**
		 **R4**
		 **R12*
		 **LR**(Return Register of Task before switching)
		 **PC**(Next Task Instruction)
		 *xPSR*
		 *=====*
		 **R5,R6,R7,R8,R9,R10,R11**(Save&restore manual)*/
	Task->_C_PSP = (unsigned int*)Task->_S_PSP_TASK;
	Task->_C_PSP --;
	*Task->_C_PSP = 0x01000000;

	Task->_C_PSP --;
	*Task->_C_PSP = (unsigned int)Task -> p_TaskEntry;//PC

	Task->_C_PSP --;
	*Task->_C_PSP = 0xFFFFFFFD;//LR

	for(int i = 0 ; i<13 ; i++){
		Task->_C_PSP --;
		*Task->_C_PSP = 0;//LR
	}
}

RTOS_Error_ID RTOS_Create_Task(TCB_RTOS* Task){

	RTOS_Error_ID Status = success;

	//Check Task size doesnt exceed PSP Size
	Task->_S_PSP_TASK = (unsigned int)OS_Control.PSP_Task_Locator;
	Task->_E_PSP_TASK = Task->_S_PSP_TASK - Task->Stack_Size;

	unsigned int Value = Task->_E_PSP_TASK;
	#if (Value <  _eheap)
	#error "Stack Size exceeds limits"
	#endif

	OS_Control.PSP_Task_Locator =Task->_E_PSP_TASK - 8;

	//Create Task PSP Stack
	RTOS_Create_Stack(Task);
	//Task State update
	Task->TaskState=Suspended;

	return Status;
}

