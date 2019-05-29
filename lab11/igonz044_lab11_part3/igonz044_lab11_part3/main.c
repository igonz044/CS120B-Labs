/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab11_part3.c
 * Exercise Description: Keypad will be used to display number pressed on LCD
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>
#include "io.c"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"

//global variables
unsigned char tempB = 0x00;
enum KeyPad_LCD{output};
	
//Initializing our state machine/function
int KeyPad_LCD_SM(int k);

int main()
{
	DDRA = 0xFF; PORTA = 0x00;//BUTTONS! input
	DDRB = 0xFF; PORTB = 0x00;//OUTPUT for the 5 LEDs (B0-B4)
	DDRC = 0xF0; PORTC = 0x0F;//KEYPAD input 
	DDRD = 0xFF; PORTD = 0x00;//LCD output
	
	// Period for the tasks
	unsigned long int SMperiod = 5;
	
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//There was only one state machine so we set the size of the array to 1
	task1.state = 0;
	task1.period = SMperiod;
	task1.elapsedTime = SMperiod;
	task1.TickFct = &KeyPad_LCD_SM;

	TimerSet(SMperiod);
	TimerOn();
	unsigned char i;
	LCD_init();
	
	while(1) {
		// Scheduler code for the SM
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime >= tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}

int KeyPad_LCD_SM(int state){
	unsigned char x;
	x = GetKeypadKey();
	switch(state){
		case output:
		switch (x) {
			case '\0': break;
			
			case '1': tempB = 0x31;
			LCD_Cursor(1);
			LCD_WriteData(tempB); break; //hex values of all the numbers and letters are outputted to tempB which is portB
			
			case '2': tempB = 0x32;
			LCD_Cursor(1); 
			LCD_WriteData(tempB ); 
			break;
			
			case '3': tempB = 0x33;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '4': tempB = 0x34;
			LCD_Cursor(1); 
			LCD_WriteData(tempB);
			break;
			
			case '5': tempB = 0x35;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '6': tempB = 0x36;
			LCD_Cursor(1); LCD_WriteData(tempB); break;
			
			case '7': tempB = 0x37;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '8': tempB = 0x38;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '9': tempB = 0x39;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case 'A': tempB = 0x41;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case 'B': tempB = 0x42;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case 'C': tempB = 0x43;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case 'D': tempB = 0x44;
			LCD_Cursor(1); 
			LCD_WriteData(tempB);
			break;
			
			case '*': tempB = 0x2A;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '0': tempB = 0x00;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			case '#': tempB = 0x23;
			LCD_Cursor(1); 
			LCD_WriteData(tempB); 
			break;
			
			default: tempB = 0x1B; 
			break; // Should never occur. Middle LED off.
		}
		state = output;
		if(tempB == 0x1B){}
			//PORTB = PORTB;
		else{
			PORTB = tempB;
		}
		break;

	}
	return state;
}
