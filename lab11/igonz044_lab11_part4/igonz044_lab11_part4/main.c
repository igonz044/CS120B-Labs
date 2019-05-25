/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab11_part4.c 
 * Exercise Description: Keypad with LCD will save numbers inputted
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

unsigned char tempB = 0x1F;
unsigned char cursorCounter = 1;

enum KeyPad_LCD_SM2_state {output} state;
int KeyPad_LCD_SM2(int k);	

int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	// Period for the tasks
	unsigned long int SM_Period = 5;

	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	//There was only one task to do so the size = 1;
	task1.state = 0; 
	task1.period = SM_Period; //Task Period.
	task1.elapsedTime =task1.period;
	task1.TickFct = &KeyPad_LCD_SM2;

	// Set the timer and turn it on
	TimerSet(SM_Period);
	TimerOn();
	LCD_init();
	LCD_DisplayString(1,"Congratulations");

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
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

int KeyPad_LCD_SM2(int state){
	unsigned char x;
	x = GetKeypadKey();
	switch(state){
		case output:
		switch (x) {
			case '\0':
			if(tempB != 0x1F){
				LCD_Cursor(cursorCounter);
				LCD_WriteData(tempB + '0');
				cursorCounter++;
				tempB = 0x1F;
			}
			tempB = 0x1F;
			break;
			
			case '1':
			tempB = 0x01;
			break;
			
			case '2':
			tempB = 0x02;
			break;
			
			case '3':
			tempB = 0x03;
			break;
			
			case '4':
			tempB = 0x04;
			break;
			
			case '5':
			tempB = 0x05;
			break;
			
			case '6':
			tempB = 0x06;
			break;
			
			case '7':
			tempB = 0x07;
			break;
			
			case '8':
			tempB = 0x08;
			break;
			
			case '9':
			tempB = 0x09;
			break;
			
			case 'A':
			tempB = 0x41 - '0';
			break;
			
			case 'B':
			tempB = 0x42 - '0';
			break;
			
			case 'C':
			tempB = 0x43 - '0';
			break;
			
			case 'D':
			tempB = 0x44 - '0';
			break;
			
			case '*':
			tempB = 0x02A - '0';
			break;
			
			case '0':
			tempB = 0x00;
			break;
			
			case '#':
			tempB = 0x23 - '0';
			break;
			
			default: tempB = 0x1B; break; // Should never occur. Middle LED off.
		}
		if(cursorCounter==17){
			cursorCounter=1;
		}
		state = output;
		PORTB=tempB;
		break;
	}
	return state;
}
