/*
 * RTOSFIFO.h
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#ifndef INC_RTOSFIFO_H_
#define INC_RTOSFIFO_H_

#include "stdio.h"
#include "SCH.h"
/*User Config */
typedef TCB_RTOS* type_t;
/*Type definitions*/
typedef struct{
	unsigned int length;
	unsigned int elements;
	type_t* head;
	type_t* base;
	type_t* tail;
}Queue_t;

typedef enum{
	Queue_Work,
	Queue_Null,
	Queue_Full,
	Queue_Empty
}Queue_t_Status;

/*APIS*/

Queue_t_Status Queue_enqueue(Queue_t*Queue,type_t element);
Queue_t_Status Queue_dequeue(Queue_t*Queue,type_t* element);
Queue_t_Status Queue_init(Queue_t*Queue,type_t* arr,unsigned int  length);

#endif /* INC_RTOSFIFO_H_ */
