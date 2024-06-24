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
#define  type_t TCB_RTOS*
/*Type definitions*/
typedef struct{
	unsigned int elements;
	type_t* head;
	type_t* base;
	type_t* tail;
	unsigned int length;
}Queue_t;

typedef enum{
	FIFO_NO_ERROR,
	FIFO_FULL,
	FIFO_EMPTY,
	FIFO_NULL,

}Queue_t_Status;

/*APIS*/

Queue_t_Status Queue_enqueue(Queue_t*Queue,type_t element);
Queue_t_Status Queue_dequeue(Queue_t*Queue,type_t* element);
Queue_t_Status Queue_init(Queue_t*Queue,type_t* arr,unsigned int  length);

#endif /* INC_RTOSFIFO_H_ */
