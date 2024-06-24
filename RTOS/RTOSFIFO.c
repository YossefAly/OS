/*
 * RTOSQueue.c
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */


#include "RTOSFIFO.h"
Queue_t_Status Queue_init (Queue_t* Queue,type_t* buff , unsigned int length){

	if(!buff )
		return FIFO_NULL;

	Queue->base = buff ;
	Queue->head = Queue->base ;
	Queue->tail = Queue->base ;
	Queue->length = length;
	Queue->elements=0;

	return FIFO_NO_ERROR;

}

/* *********************************** */

Queue_t_Status Queue_enqueue (Queue_t* Queue,type_t item){

	/* Queue null*/

	if (!Queue->base || !Queue->length)
		return FIFO_NULL;
	/*Queue is full*/

	/* Queue full */
	if ((Queue->head == Queue->tail) && (Queue->elements == Queue->length))
		return FIFO_FULL;

	*(Queue->tail)=item;
	Queue->elements++;

	/*for circular Queue again */

	/* circular enqueue */
	if (Queue->tail == (((unsigned int)Queue->base + (4*Queue->length )) - 4 ))
		Queue->tail = Queue->base;
	else
		Queue->tail++;

	return FIFO_NO_ERROR;


}

/* *********************************** */

Queue_t_Status Queue_dequeue (Queue_t* Queue, type_t* item){
	/* check Queue valid */
	if (!Queue->base || !Queue->length)
		return FIFO_NULL;

	/* Queue empty */
	if (Queue->head == Queue->tail)
		return FIFO_EMPTY;



	*item = *(Queue->head);
	Queue->elements--;

	/* circular dequeue */
	if (Queue->head == (((unsigned int)Queue->base + (4*Queue->length )) - 4 ))
		Queue->head = Queue->base;
	else
		Queue->head++;

	return FIFO_NO_ERROR;
}




