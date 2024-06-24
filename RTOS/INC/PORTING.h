/*
 * PORTING.h
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */

#ifndef INC_PORTING_H_
#define INC_PORTING_H_

#include "core_cm3.h"


extern int _estack ;
extern int _eheap  ;
#define MainStackSize 	3072


#define SET_OS_PSP(add)              __asm volatile("mov r0,%0 \n\t msr PSP,r0" : : "r" (add) )
#define GET_OS_PSP(add)              __asm volatile("mrs r0,PSP \n\t mov %0,r0"   : "=r" (add) )

#define SET_MSP_PSP          __asm volatile("mrs r0, CONTROL \n\t mov r1,#0x02 \n\t orr r0,r0,r1 \n\t msr CONTROL,r0")
#define SET_PSP_MSP          __asm volatile("mrs r0, CONTROL \n\t mov r1,#0x05 \n\t and r0,r0,r1 \n\t msr CONTROL,r0")



//clear Bit 0 CONTROL register
#define SWITCH_CPU_ACCESS_priveleged   		__asm(" mrs r3, CONTROL  \n\t" \
										" lsr r3,r3,#0x1   \n\t"       \
										" lsl r3,r3,#0x1   \n\t"	   \
										" msr CONTROL, r3");

//set Bit 0 CONTROL register
#define SWITCH_CPU_ACCESS_unpriveleged		__asm(" mrs r3, CONTROL  \n\t" \
											  " orr r3,r3,#0x1   \n\t" \
											  " msr CONTROL, r3 ");

void trigger_OS_PendSV();
void Start_Ticker();

#endif /* INC_PORTING_H_ */
