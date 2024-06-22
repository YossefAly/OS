/*
 * PORTING.c
 *
 *  Created on: Jun 22, 2024
 *      Author: ALY
 */


#include "PORTING.h"

void 	HardFault_Handler(void){while(1);}
void	MemManage_Handler(void){while(1);}
void	BusFault_Handler(void){while(1);}
void	UsageFault_Handler(void){while(1);}

__attribute ((naked)) void SVC_Handler(){

	__asm ("TST LR,#0x04 \n\t"
			"ITE EQ \n\t"
			"MRSEQ R0,MSP \n\t"
			"MRSNE R0,PSP \n\t"
			"B SVC_Handler_C");



}



void HW_init(){
	// Initialize Clock tree (RCC -> Systick & CPU) 8MHZ
	// 8 MHZ
	// 1 count -> 0.125us
	// x Tick -> 1ms
	// x = 8000 count

}
