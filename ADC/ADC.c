// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Javier Pardo & Sergis Varda 


#include "tm4c123gh6pm.h"

// This function should initialize ADC sampling at the lowest possible rate
// use a sequencer and step of your choice

void ADC_Init(void){ volatile unsigned long delay;

	SYSCTL_RCGC2_R |= 0x00000010;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R &= ~0x04;
	GPIO_PORTE_AFSEL_R |= 0x04;
	GPIO_PORTE_DEN_R &= ~0x04;
	GPIO_PORTE_AMSEL_R |= 0x04;
	SYSCTL_RCGC0_R |= 0x00010000;
	delay = SYSCTL_RCGC2_R;
	SYSCTL_RCGC0_R |= ~0x00000300;
	ADC0_SSPRI_R = 0x0123;
	ADC0_ACTSS_R &= ~0x0008;
	ADC0_EMUX_R &= ~0xF000;
	ADC0_SSMUX3_R &= ~0x000F;
	ADC0_SSMUX3_R += 1;
	ADC0_SSCTL3_R = 0x0006;
	ADC0_ACTSS_R |= 0x0008;

}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC_In(void){  

	unsigned long data;
	ADC0_PSSI_R = 0x008;
	while((ADC0_RIS_R & 0x08) == 0) {};
	data = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R = 0x0008;
	return data;
}


