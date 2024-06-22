/*
 * RTOSFIFO.c
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */


#include "RTOSFIFO.h"

Queue_t_Status Queue_enqueue(Queue_t*Queue,type_t element){
	/*Validation*/
	if(!Queue->base||!Queue->head||!Queue->tail)
		return Queue_Null;
	else if(Queue->elements==Queue->length)
		return Queue_Full;
	else{
		*(Queue->head)++=element;Queue->elements++;return Queue_Work;}
}
Queue_t_Status Queue_dequeue(Queue_t*Queue,type_t* element){
	/*Validation*/
	if(!Queue->base||!Queue->head||!Queue->tail)
		return Queue_Null;
	else if(Queue->elements==0)
		return Queue_Empty;
	else{
		*element=*(Queue->tail)++;Queue->elements--;return Queue_Work;}
}
Queue_t_Status Queue_init(Queue_t*Queue,type_t* arr,unsigned int length){
	/*Validation*/
	if(arr==NULL)
		return Queue_Null;
	Queue->base=arr;
	Queue->head=arr;
	Queue->tail=arr;
	Queue->length=length;
	Queue->elements=0;
	return Queue_Work;
}
