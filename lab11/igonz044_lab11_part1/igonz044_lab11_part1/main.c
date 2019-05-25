/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab11_part1.c 
 * Exercise Description: LEDS show which Keypad numbers were pressed
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

enum KeyTick_states {wait, zero, one, two, three, four, five, six, seven, eight, nine, A, B, C, D, pound, star} state;

int KeyTick(int k);
int KT_state;
int nextState;

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;//OUTPUT for the 5 LEDs (B0-B4)
	DDRC = 0xF0; PORTC = 0x0F;//KEYPAD PC7:C4 outputs 0s, PC3:C0 inputs 1s
	
	int x;
	unsigned char i;
	const unsigned char tasksSize = 2;
	
	static task task1;
	task *tasks[] = { &task1};

	task1.state = zero;
	task1.period = 1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &KeyTick;

	TimerSet(1000);
	TimerOn();

	while(1) {
		x = GetKeypadKey();
		task1.state = x;
		for(i = 0; i<tasksSize; i++){
			if(task1.elapsedTime >= task1.period){
				task1.state = task1.TickFct(task1.state);
				task1.elapsedTime = 0;
			}
			task1.elapsedTime += task1.period;
		}

		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

int KeyTick(int KT_state)
{
	switch (KT_state) {
		
		case wait:
		PORTC = 0xEF; // Enable col 4 with 0, disable others with 1?s
		asm("nop"); // add a delay to allow PORTC to stabilize before checking
		if (GetBit(PINC,0)==0) { nextState = one;}
		if (GetBit(PINC,1)==0) { nextState = four;}
		if (GetBit(PINC,2)==0) { nextState = seven;}
		if (GetBit(PINC,3)==0) { nextState = star;}

		// Check keys in col 2
		PORTC = 0xDF; // Enable col 5 with 0, disable others with 1?s
		asm("nop"); // add a delay to allow PORTC to stabilize before checking
		if (GetBit(PINC,0)==0) { nextState = two; }
		if (GetBit(PINC,1)==0) { nextState = five; }
		if (GetBit(PINC,2)==0) { nextState = eight; }
		if (GetBit(PINC,3)==0) { nextState = zero;}

		// Check keys in col 3
		PORTC = 0xBF; // Enable col 6 with 0, disable others with 1?s
		asm("nop"); // add a delay to allow PORTC to stabilize before checking
		if (GetBit(PINC,0)==0) { nextState = three; }
		if (GetBit(PINC,1)==0) { nextState = six; }
		if (GetBit(PINC,2)==0) { nextState = nine; }
		if (GetBit(PINC,3)==0) { nextState = pound; }

		PORTC = 0x7F; // Enable col 6 with 0, disable others with 1?s
		asm("nop"); // add a delay to allow PORTC to stabilize before checking
		if (GetBit(PINC,0)==0) { nextState = A; }
		if (GetBit(PINC,1)==0) { nextState = B; }
		if (GetBit(PINC,2)==0) { nextState = C; }
		if (GetBit(PINC,3)==0) { nextState = D; }
		break;

		case one: nextState = wait; break;
		case two: nextState = wait; break;
		case three: nextState = wait; break;
		case four: nextState = wait; break;
		case five: nextState = wait; break;
		case six: nextState = wait; break;
		case seven: nextState = wait; break;
		case eight: nextState = wait; break;
		case nine: nextState = wait; break;
		case A: nextState = wait; break;
		case B: nextState = wait; break;
		case C: nextState = wait; break;
		case D: nextState = wait; break;

		case star: nextState = wait; break;
		case zero: nextState = wait; break;
		case pound: nextState = wait; break;

		default: nextState = wait; break;
	}

	switch (KT_state) {
		case wait: PORTB = PORTB; break;
		case one: PORTB = 0x01;  break; // hex equivalent
		case two: PORTB = 0x02;  break;
		case three: PORTB = 0x03;  break;
		case four: PORTB = 0x04; break;
		case five: PORTB = 0x05; break;
		case six: PORTB = 0x06;  break;
		case seven: PORTB = 0x07;  break;
		case eight: PORTB = 0x08;  break;
		case nine: PORTB = 0x09;  break;

		case A: PORTB = 0x0A;  break;
		case B: PORTB = 0x0B;  break;
		case C: PORTB = 0x0C;  break;
		case D: PORTB = 0x0D;  break;

		case star: PORTB = 0x0E; break;
		case zero: PORTB = 0x00;   break;
		case pound: PORTB = 0x0F;  break;
		default: PORTB = 0x1B;  break;
	}

	KT_state = nextState;
	return KT_state;
}