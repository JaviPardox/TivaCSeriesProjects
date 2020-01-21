
// TrafficLight.c
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate
// a traffic light.
// Made by: Javier Pardo & Sergis Varda
// created: 10/5/17
// 
// 

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1 Program 6.8, Example 6.4
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Volume 2 Program 3.1, Example 3.1

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "PLL.h"
#include "SysTick.h"



// add any needed port definitions 
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
	

#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// Linked data structure
// define your structure here
struct state{    //declaration of the structure state
	int output;
	int time;
	struct state *next[16];  //16 correspond to the number of inputs possible
};

typedef struct state stype; //instead of using state to call the structure we change it to stype

stype FSM[7];  //number of states 
//define your states here e.g. #define stateName 0, etc.
#define GoW &FSM[0]
#define WaitW &FSM[1]
#define Go1st &FSM[2]
#define Wait1st &FSM[3]
#define GoPedW &FSM[4]
#define GoPed1st &FSM[5]
#define GoW3 &FSM[6]

//Declare your states here 
stype FSM[7] = {
	{0x30,700,{GoW,GoW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW}},
	{0x50,200,{Go1st,Go1st,Go1st,Go1st,GoPedW,GoPedW,GoPedW,GoPedW,GoPed1st,GoPed1st,GoPed1st,GoPed1st,GoPedW,GoPedW,GoPedW,GoPedW}},
	{0x84,700,{Wait1st,Wait1st,Go1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st,Wait1st}},
	{0x88,200,{GoW,GoW,GoW,GoW,GoPedW,GoPedW,GoPedW,GoPedW,GoPed1st,GoPed1st,GoPed1st,GoPed1st,GoPed1st,GoPed1st,GoPed1st,GoPed1st}},
	{0x93,400,{GoW,GoW,Go1st,GoW,GoPedW,GoW,Go1st,GoW,GoPed1st,GoW,GoPed1st,Go1st,GoPed1st,GoW,Go1st,GoW}},
	{0x31,400,{GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3,GoW3}},
	{0x30,300,{GoW,GoW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW,WaitW}}
};



int main(void){ 
	volatile unsigned long delay;
	unsigned long input;
	stype *ptr;        //making a pointer pointing to our linked list
  PLL_Init();       // 80 MHz, Program 10.1
  SysTick_Init();   // Program 10.2
 
  
	// initialize ports here
	SYSCTL_RCGC2_R |= 0x12;
	
	GPIO_PORTE_DIR_R &= ~0x1E; //pins 1 2 3 4 as input
	GPIO_PORTE_AFSEL_R &= ~0xFF; 
	GPIO_PORTE_AMSEL_R &= ~0xFF;
	GPIO_PORTE_DEN_R |= 0x1E; //digital enable used pins
	
  GPIO_PORTB_DIR_R |= 0xFF; //pins 0 1 2 3 4 5 6 7 as output
	GPIO_PORTB_AFSEL_R &= ~0xFF; 
	GPIO_PORTB_AMSEL_R &= ~0xFF;
	GPIO_PORTB_DEN_R |= 0xFF; //digital enable used pins
	

	ptr = GoW; //start by setting the pointer to point to our first state
	
  while(1){
  //your code goes here to move from one state to another. 
		GPIO_PORTB_DATA_R |= ptr->output; //output whatever that state holds in the ouput variable
		SysTick_Wait10ms(ptr->time);      //take the value of time from the state and wait 
		GPIO_PORTB_DATA_R &= ~ptr->output; //turn off whatever lights were on
		input = GPIO_PORTE_DATA_R & 0x1E;  //mask out the value of the data_r of port E which is the input
		ptr = ptr->next[input >> 1];   //go to the next state depending on the input,
		                               //also shift to the right by one as we are not using pin 0 for our input so it would translate it to wrong inputs
  }
}

