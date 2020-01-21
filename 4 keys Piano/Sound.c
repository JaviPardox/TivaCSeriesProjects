// Sound.c, 
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Javier Pardo & Sergis Varda
// Hardware connections
#include "tm4c123gh6pm.h"
#include "dac.h"
// put code definitions for the software (actual C code)
// this file explains how the module works
const unsigned long sin_wave[] = {16,19,22,25,27,29,30,31,31,30,29,27,25,22,19,16,12,9,6,4,2,1,0,0,1,2,4,6,9,12};
int i = 0;
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: Initial interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(unsigned long period){
	DAC_Init();
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = period - 1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0xA0000000; //priority 5
	NVIC_ST_CTRL_R = 0x00000007;  //enable with core clock and interrupts
}

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disables sound output
// Output: none
void Sound_Play(unsigned long freq){
	int counts;
	if(freq == 0)
	{
		NVIC_ST_RELOAD_R = 0;
	  NVIC_ST_CURRENT_R = 0;
	}
	else{
  counts = 80000000/freq;
  NVIC_ST_RELOAD_R = counts/30;  //(period/#elements) * busf(80Mhz)
	NVIC_ST_CURRENT_R = 0;
	}		//reset current with new reolad
}



// Interrupt service routine
// Executed periodically, the actual period
// determined by the current Reload.
void SysTick_Handler(void){
	DAC_Out(sin_wave[i]);
	i = (i + 1)%30;
}


