// Sound.h
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Javier Pardo & Sergis Varda
// Hardware connections

// Header files contain the prototypes for public functions
// this file explains what the module does

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
// Output: none
void Sound_Init(unsigned long period);


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//         input of zero disable sound output
// Output: none
void Sound_Play(unsigned long period);
