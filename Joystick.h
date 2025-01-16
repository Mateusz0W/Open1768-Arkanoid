#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "Driver_SPI.h"


/*
Driver_SPI0

SCK (Clock): P0.15
MOSI: P0.18
MISO: P0.17
SS (Slave Select): P0.16
*/

/* SPI Driver */
extern ARM_DRIVER_SPI Driver_SPI0;
ARM_DRIVER_SPI* SPIdrv = &Driver_SPI0;


uint8_t tx_data[5] = {0x00, 0x00, 0x00, 0x00, 0x00}; // Dummy bytes


void timer0Init(){
    LPC_SC->PCONP |= (1<<1);
    LPC_SC->PCLKSEL0 &= ~(0x3<<3);
    LPC_TIM0->PR = 25-1;// SystemCoreClock / 1000000 - 1; // Prescaler  1 MHz (1 us)
    LPC_TIM0->TCR = 0x02;
}


void SPIInit(){
     
     /* Initialize the SPI driver */
     SPIdrv->Initialize(NULL);
     /* Power up the SPI peripheral */
    SPIdrv->PowerControl(ARM_POWER_FULL);
     /* Configure the SPI to Master, 8-bit mode @10000 kBits/sec */
     /*seting up clock to 100kHz
     work mode 0  ARM_SPI_CPOL0_CPHA0 
     */
    SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8),100000 /*1000000*/); 
    /* SS line = INACTIVE = HIGH */
    SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}
void delayMicroseconds(unsigned int microseconds){
    LPC_TIM0->TCR = 0x02; //Reset Timer

	LPC_TIM0->TCR = 0x01; //Enable timer

	while(LPC_TIM0->TC < microseconds); //wait until timer counter reaches the desired delay

	LPC_TIM0->TCR = 0x00; //Disable timer
}
void read(uint8_t *data){
    /* SS line = ACTIVE = LOW */
    SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
    delayMicroseconds(15);
    SPIdrv->Transfer(tx_data, data, 5);
		uint32_t a=SPIdrv->GetStatus().busy;
		/*while(a==1){
		 a=SPIdrv->GetStatus().busy;
		LPC_UART0->THR = 'a';
		}*/
    /* SS line = INACTIVE = HIGH */
    SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
        
}

void readJoystick(int *X,int *Y,uint8_t *data){
	//char c[20];
	read(data);
  *X = data[0]; // X has a 10-bit format
  *X |= (data[1] << 8);
  *Y = data[2]; // Y has a 10-bit format
  *Y |= (data[3] << 8);
	//sprintf(c, "%x %x /// %x %x  ??? %x\n\t ", data[0],data[1],data[2],data[3],data[4]);
	//sprintf(c," %d\n ",X);				
	//send(c);
	//LPC_UART0->THR = '\r';
}
#endif
