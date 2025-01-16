#ifndef UART_H
#define UART_H

#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "Board_LED.h" 


void set_up_UART0()
{
	// change pin configuration for transmition recieving for UART
	PIN_Configure(0,2,PIN_FUNC_1,PIN_PINMODE_TRISTATE,PIN_PINMODE_NORMAL);
	PIN_Configure(0,3,PIN_FUNC_1,PIN_PINMODE_TRISTATE,PIN_PINMODE_NORMAL);
	
	// 9600 boundrate

	LPC_UART0->LCR = (1<<7);
	LPC_UART0->DLL = 163;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR = 3;
}

void send(char *text)
{
	for(int i=0; *(text + i) != '\0';i++)
	LPC_UART0->THR = *(text + i);
}

#endif
