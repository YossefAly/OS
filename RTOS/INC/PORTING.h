/*
 * PORTING.h
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#ifndef INC_PORTING_H_
#define INC_PORTING_H_

#include "core_cm3.h"

extern  int _estack;

extern  int _eheap;


#define MainStackSize 3072


#define SET_OS_PSP(ADD) __asm volatile("mov R0,%0 \n\t" \
										"msr PSP,R0" \
										:\
										:"r" (ADD) )

#define GET_OS_PSP(ADD) __asm volatile("mrs R0,PSP \n\t" \
										"mov %0,R0" \
										:"=r" (ADD))

//Set bit 1 in Control register
#define SET_MSP_PSP __asm("mrs R0,CONTROL \n\t" \
						  "mov R1,#0x02 \n\t "\
						  "orr R0,R1,R0 \n\t "\
						  "msr CONTROL,R0")

//Clear bit 1 in Control register
#define SET_PSP_MSP __asm( "mrs R0,CONTROL \n\t" \
						  "mov R1,#0xFFFFFFFD \n\t "\
						  "and R0,R1,R0 \n\t "\
						  "msr CONTROL,R0")


//Clear bit 0 in Control register
#define SWITCH_CPU_ACCESS_priveleged  __asm("MRS R0,CONTROL \n\t "\
			  	  	  	  	  	  	  	  	"AND R0,R0,#0xFFFFFFFFE \n\t"\
											"MSR CONTROL,R0");\
//Set bit 0 in Control register
#define SWITCH_CPU_ACCESS_unpriveleged __asm("MRS R0,CONTROL \n\t "\
			  	  	  	  	  	  	  	  	 "ORR R0,R0,#0x01 \n\t"\
											 "MSR CONTROL,R0");\


void trigger_PendSV();
unsigned char SysTickLED ;
void Start_Ticker();


#endif /* INC_PORTING_H_ */
