// init.c
// Runs on TM4C123
// Program written by: Javier Pardo & Sergis Varda
// Brief description of the program:
// Piano with 4 keys, this file contains the initialization
// With the help of Dr. Hadil Mustafa   
// Hardware connections

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Sound.h"
#include "Piano.h"
#include "dac.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);

int main(void){ 
  //unsigned long Data;
	int delay;
  PLL_Init();   	// bus clock at 80 MHz
  // all initializations go here
	SYSCTL_RCGC2_R |= 0x0A;
	delay = 2;
	Sound_Init(50000000);
	Piano_Init();
  EnableInterrupts();
  while(1){                
// Do not modify this section
	    WaitForInterrupt();	
  }             
}


