// Piano.c
// This software configures the off-board piano keys
// Runs on TM4C123
// Program written by: Javier Pardo & Sergis Varda
// 
//  
// 
// 
// Hardware connections

#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "dac.h"
// put code definitions for the software (actual C code)
// this file explains how the module works

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Piano_Init(void){ 
  GPIO_PORTD_DIR_R &= ~0x0F;
	GPIO_PORTD_AFSEL_R &= ~0xFF;          
	GPIO_PORTD_AMSEL_R &= ~0xFF;
	GPIO_PORTD_DEN_R |= 0x0F;
	GPIO_PORTD_IS_R &= ~0x0F;
	GPIO_PORTD_IBE_R |= 0x0F;              //double edge trigger
	GPIO_PORTD_ICR_R = 0x0F;
	GPIO_PORTD_IM_R  |= 0x0F;
	NVIC_PRI0_R = (NVIC_PRI0_R & 0x00FFFFFF) | 0x40000000; //PRIORITY 2
	NVIC_EN0_R = 0x00000008;
}
// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2 and 0x08 is just key3
unsigned long Piano_In(void){ 
	unsigned long mask;
  mask = GPIO_PORTD_DATA_R & 0x0F;
  return mask;
}


void GPIOPortD_Handler(void)
{
	unsigned long maskD;  //ris icr resgusters
  maskD = Piano_In();
	if(maskD == 0x01)
	{
		Sound_Play(262);
	}
	else if(maskD == 0x02)
	{
		Sound_Play(294);
	}
	else if(maskD == 0x04)
	{
		Sound_Play(330);
	}
	else if(maskD == 0x08)
	{
		Sound_Play(349);
	}
	else
	{
		Sound_Play(0);
	}
	GPIO_PORTD_ICR_R = 0x0F;
}

