/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab11_part2.c 
 * Exercise Description: LCD will dsiplay "CS120B is Legend... wait for it DARY!" 
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

enum KeyTick_states {wait, zero, one, two, three, four, five, six, seven, eight, nine,
    A, B, C, D, pound, star} state;
enum Display_states {output, increment};

int KeyTick(int k);
int Legendary(int L);
int KT_state;
int nextState;
int LG_state;
int LG_next;
unsigned short scroll_counter = 1;
unsigned short array_counter = 1;
unsigned short output_counter = 0;
unsigned short display_delay = 0;
unsigned char DisplayArray [80] = {' ','C', 'S', '1', '2', '0', 'B', ' ', 'I', 'S', ' ', 'L', 'E', 'G', 'E', 'N', 'D', ' ', 'W', 'A', 'I', 'T', ' ', 'F', 'O', 'R', ' ', 'I', 'T', '.', '.', '.', '.', '.', '.', '.', '.', 'D', 'A', 'R', 'Y', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTB = 0x00; //
	DDRA = 0xFF; PORTA = 0x00; //

	int x;
	unsigned char i;
	const unsigned char tasksSize = 2;
	static task task1, task2;
	task *tasks[] = {&task1, &task2};

	task1.state = zero;
	task1.period = 1000;
	task1.elapsedTime = task1.period;
	task1.TickFct = &KeyTick;
	
	task2.state = output;
	task2.period = 10;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Legendary;

	TimerSet(10);
	TimerOn();
	LCD_init();

	while(1) {
		x = GetKeypadKey();
		for(i = 0; i < tasksSize; i++){
			if(tasks[i]->elapsedTime >= tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += tasks[i]->period;
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

//The display can only output 16 characters at a time!
int Legendary(int LG_state)
{
	switch (LG_state) {

		case output:
		if(display_delay > 80)
		{
			LG_next = increment;
		}
		else
		{
			LG_next = output;
		}
		break;

		case increment:
		LG_next = output;
		break;

		default:
		LG_next = output;
		break;
	}
	
	switch (LG_state) {
		case output:
		if(display_delay < 32)
		{
			LCD_Cursor(scroll_counter);
			LCD_WriteData(DisplayArray[array_counter]);
			scroll_counter++;
			array_counter++;
		}
		display_delay++;
	
		break;

		case increment: 
		output_counter++;
		scroll_counter = 1;
		array_counter  = output_counter + 1;
		display_delay = 0;
		if(output_counter > 47 )
		{
			output_counter = 0;
		}
		break;

		default: 
		LCD_DisplayString(1, "err0r!");
		break;
	}

	LG_state = LG_next;
	return LG_state;
}