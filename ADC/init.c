// init.c
// this lab creates a position measurment software that outputs the result on LCD
// The software samples analog input signal and use calibration to output the position
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at a particular period.
// Javier Pardo & Sergis Varda 
// 11/30/2017
// With the help of Dr. Hadil Mustafa


#include "Nokia5110.h"
#include "pll.h"
#include "ADC.h"
#include "tm4c123gh6pm.h"

unsigned long Convert(unsigned long input);
 
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
	

void PortF_Init(void){unsigned long volatile delay;

   	SYSCTL_RCGC2_R |= 0x20; 
  	delay = 2; 
   	GPIO_PORTE_DIR_R |= 0x04;  //PF2 
	  GPIO_PORTE_AMSEL_R &= ~0xFF; 
	  GPIO_PORTE_AFSEL_R &= ~0xFF;  
	  GPIO_PORTE_DEN_R |= 0x04; 
}

void Systick_Init(void){
	
		NVIC_ST_CTRL_R = 0;                        //disable systick during setup
		NVIC_ST_RELOAD_R = 2000000;                //load period into reload register
		NVIC_ST_CURRENT_R = 0;                     //reset current register
	  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x60000000; //adc seq 1 priority to 3
		NVIC_ST_CTRL_R = 0x00000007;               //enable systick with core clock and interrupts
		
}


// declaire any global variable here
unsigned long sample;


// write main1 function to test ADC_Init and ADC_In functions
// single step this program and look at Data
void Systick_Handler(void){
	GPIO_PORTF_DATA_R |= 0x04;
	sample = Convert(ADC_In());
	GPIO_PORTF_DATA_R &= ~0x04;
}


int main1(void){ 

PLL_Init(); 		// adjust PLL.c to run the system at a frequency of your choice (50MHz-80MHz)	
ADC_Init();
	while(1){
	  sample = ADC_In();
	}	
}



int main(void){ 
  PLL_Init();        
  PortF_Init();
  ADC_Init(); 
	//Systick_Init();
	//EnableInterrupts();
  while(1){  
		GPIO_PORTF_DATA_R |= 0x04;
		sample = Convert(ADC_In());
		GPIO_PORTF_DATA_R &= ~0x04;
		//WaitForInterrupt();

  }
} 
unsigned long Convert(unsigned long input){
  return (3277 * input + 546824) >> 13;
}


 
