/*
 * SCH.c
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#include "SCH.h"
#include "RTOSFIFO.h"

struct {
	TCB_RTOS* OSTasks[5];//SCH Table
	unsigned int _S_MSP_TASK;
	unsigned int _E_MSP_TASK;
	unsigned int PSP_Task_Locator;
	unsigned int NumberOfActiveTasks;
	TCB_RTOS* CurrentTask;
	TCB_RTOS* NextTask;
	enum{
		OS_Suspend,OS_run
	}OS_mode;

}OS_Control;

typedef enum{Activate,Terminate,Wait}SVC_ID;

Queue_t Ready_Queue;
TCB_RTOS* Ready_Queue_FIFO[100];


void Bubble_Sort(){

	 int i, j,n;
	 n=OS_Control.NumberOfActiveTasks;
	    char swapped;
	    for (i = 0; i < n - 1; i++) {
	        swapped = 0;
	        for (j = 0; j < n - i - 1; j++) {
	            if (OS_Control.OSTasks[j]->Priority > OS_Control.OSTasks[j+1]->Priority) {
	            	TCB_RTOS* temp = OS_Control.OSTasks[j];
	            	OS_Control.OSTasks[j] =  OS_Control.OSTasks[j+1];
	            	OS_Control.OSTasks[j+1] = temp;
	                swapped = 1;
	            }
	        }

	        // If no two elements were swapped
	        // by inner loop, then break
	        if (swapped == 0)
	            break;
	    }
}

/*Excute specific  OS service*/
void SVC_Handler_C(int * SFargs){


	unsigned int __stacked_PC = SFargs[6];

	unsigned char SvcNumber = *((unsigned char*)((unsigned char*)(__stacked_PC)-2));

	switch (SvcNumber){

	case 0://Activate Task
			RTOS_Update_SCH();//Update Table
			if(OS_Control.OS_mode == OS_run){
				if(strcmp(OS_Control.CurrentTask->TaskName,"IdleTask")!=0)
					Next();
				 trigger_PendSV();
			}
		break;

	case 1://Terminate Task
				trigger_PendSV();
		break;

	case 2://Wait Task
				trigger_PendSV();
		break;

	case 3:

		break;

	default:

		break;

	}
}
void Next(){
	//if Ready Queue is empty && OS_Control->currentTask != suspend
		if (Ready_Queue.elements == 0 && OS_Control.CurrentTask->TaskState != Suspended) //Queue_EMPTY
		{
			OS_Control.CurrentTask->TaskState = Running ;
			//add the current task again(round robin)
			Queue_dequeue(&Ready_Queue, &OS_Control.CurrentTask);
			OS_Control.NextTask = OS_Control.CurrentTask ;
		}else
		{
			Queue_dequeue(&Ready_Queue, &OS_Control.NextTask);
			OS_Control.NextTask->TaskState = Running ;
			//update Ready queue (to keep round robin Algo. happen)
			if ((OS_Control.CurrentTask->Priority == OS_Control.NextTask->Priority )&&(OS_Control.CurrentTask->TaskState != Suspended))
			{
				Queue_dequeue(&Ready_Queue, &OS_Control.CurrentTask);
				OS_Control.CurrentTask->TaskState = Ready ;
			}
		}
}

void RTOS_SVC_SET(SVC_ID ID){

	switch(ID){

	case Activate:

		 __asm("SVC 0x00");
		break;
	case Terminate:

		 __asm("SVC 0x01");
		break;
	case Wait:
		 __asm("SVC 0x02");
		break;
	case 3:
		 __asm("SVC 0x03");
		break;
	default: break;
	}

}



__attribute ((naked)) void PendSV_Handler()
{
	//====================================
	//Save the Context of the Current Task
	//====================================
	//Get the Current Task "Current PSP from CPU register" as CPU Push XPSR,.....,R0
	GET_OS_PSP(OS_Control.CurrentTask->_C_PSP);

	//using this _C_PSP (Pointer) to store (R4 to R11)
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r4 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r5 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r6 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r7 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r8 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r9 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r10 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP-- ;
	__asm volatile("mov %0,r11 "
			: "=r" (*(OS_Control.CurrentTask->_C_PSP))  );

	//save the current Value of PSP
	//already saved in _C_PSP



	//====================================
	//Restore the Context of the Next Task
	//====================================
	if (OS_Control.NextTask != NULL){
	OS_Control.CurrentTask = OS_Control.NextTask ;
	OS_Control.NextTask = NULL ;
	}

	__asm volatile("mov r11,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r10,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r9,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r8,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r7,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r6,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r5,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;
	__asm volatile("mov r4,%0 "
			:
			: "r" (*(OS_Control.CurrentTask->_C_PSP))  );
	OS_Control.CurrentTask->_C_PSP++ ;

	//update PSP and exit
	SET_OS_PSP(OS_Control.CurrentTask->_C_PSP);
	__asm volatile("BX LR");

}

void RTOS_Create_MStack(){

	OS_Control._S_MSP_TASK = &_estack;
	OS_Control._E_MSP_TASK = OS_Control._S_MSP_TASK - MainStackSize;
	OS_Control.PSP_Task_Locator = (OS_Control._E_MSP_TASK - 8);

}

unsigned char IdleTaske;
void IdleTask(){
	IdleTaske^=1;
	while(1){
		IdleTaske^=1;
	__asm("NOP \n\t");
	}

}


RTOS_Error_ID RTOS_init(){
	RTOS_Error_ID Status;

	//Update Os mode
	OS_Control.OS_mode=OS_Suspend;


	//Specify main stack for OS
	RTOS_Create_MStack();


	//Create OS ready Queue
	if(Queue_init(&Ready_Queue,Ready_Queue_FIFO, 100)==Queue_Work)
	{Status=success;}

	//Create Idle Task
	strcpy(Ready_Queue_FIFO[0]->TaskName,"IdleTask");//Idle Task is always Task no.1 in Queue (0 index)
	Ready_Queue_FIFO[0]->Priority = 255;
	Ready_Queue_FIFO[0]->p_TaskEntry = IdleTask;
	Ready_Queue_FIFO[0]->Stack_Size = 100 ;

	Status=RTOS_Create_Task(Ready_Queue_FIFO[0]);

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
	Task->_C_PSP = Task->_S_PSP_TASK;
	Task->_C_PSP --;
	*(Task->_C_PSP) = 0x01000000;

	Task->_C_PSP --;
	*(Task->_C_PSP) = (unsigned int)Task -> p_TaskEntry;//PC

	Task->_C_PSP --;
	*(Task->_C_PSP) = 0xFFFFFFFD;//LR

	for(int i = 0 ; i<13 ; i++){
		Task->_C_PSP --;
		*(Task->_C_PSP) = 0;//LR
	}
}

RTOS_Error_ID RTOS_Create_Task(TCB_RTOS* Task){

	RTOS_Error_ID Status = success;

	//Check Task size doesnt exceed PSP Size
	Task->_S_PSP_TASK = OS_Control.PSP_Task_Locator;
	Task->_E_PSP_TASK = Task->_S_PSP_TASK - Task->Stack_Size;


	OS_Control.PSP_Task_Locator =Task->_E_PSP_TASK - 8;

	//Create Task PSP Stack
	RTOS_Create_Stack(Task);

	OS_Control.OSTasks[OS_Control.NumberOfActiveTasks] = Task;
	OS_Control.NumberOfActiveTasks++;
	//Task State update
	Task->TaskState=Suspended;

	return Status;
}
RTOS_Error_ID RTOS_Activate_Task(TCB_RTOS* Task){

	RTOS_Error_ID Status = success ;

	Task->TaskState = Waiting ;

	RTOS_SVC_SET(Activate);

	return Status;
}
RTOS_Error_ID RTOS_Terminate_Task(TCB_RTOS* Task){

	RTOS_Error_ID Status = success ;


	Task->TaskState = Suspended ;

	return Status;

}
void RTOS_Update_SCH(){

	TCB_RTOS* temp = NULL;

	TCB_RTOS * Current;
	TCB_RTOS * next;


	int i = 0;

	//Bubble sort the priority
	Bubble_Sort();
	//Empty Ready Queue
	while(Queue_dequeue(&Ready_Queue, &temp) != Queue_Empty);

	//Update Ready Queue

	while(i < OS_Control.NumberOfActiveTasks){

		Current = OS_Control.OSTasks[i];
		next = OS_Control.OSTasks[i+1];

		if(Current->TaskState != Suspended){

			if(next->TaskState == Suspended){

				Queue_enqueue(&Ready_Queue, Current);
				Current->TaskState = Ready;
				break;
			}
			if(Current->Priority <= next -> Priority){

				Queue_enqueue(&Ready_Queue, Current);
				Current->TaskState = Ready;
			}



		i++;

	}

	}
}

RTOS_Error_ID RTOS_Start(){

	OS_Control.OS_mode = OS_run ;
	//Set Default "Current Task =Idle Task"
	OS_Control.CurrentTask = Ready_Queue_FIFO[0] ;
	//Activate IDLE Task
	RTOS_Activate_Task(Ready_Queue_FIFO[0]);
	//Start Ticker
	Start_Ticker(); // 1ms

	SET_OS_PSP(OS_Control.CurrentTask->_C_PSP);
	//Switch Thread Mode SP from MSP to PSP
	SET_MSP_PSP;
	SWITCH_CPU_ACCESS_unpriveleged;
	Ready_Queue_FIFO[0]->p_TaskEntry();

}




