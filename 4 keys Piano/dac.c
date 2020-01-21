// dac.c
// This software configures DAC output
// Runs on TM4C123
// Program written by: Javier Pardo & Sergis Varda


#include "tm4c123gh6pm.h"
// put code definitions for the software (actual C code)
// this file explains how the module works

// **************DAC_Init*********************
// Initialize 5-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	GPIO_PORTB_DIR_R |= 0x7C;
	GPIO_PORTB_AFSEL_R &= ~0xFF;
	GPIO_PORTB_AMSEL_R &= ~0xFF;
	GPIO_PORTB_DEN_R |= 0x7C;
}

// **************DAC_Out*********************
// output to DAC
// Input: 5-bit data, 0 to 31 
// Output: none
void DAC_Out(unsigned long data){
	GPIO_PORTB_DATA_R = data << 2;
}
