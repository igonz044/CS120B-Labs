/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab4_part1.c
 * Created: 4/12/2019 9:23:32 AM
 * Author : ucrcse
 * Exercise Description: Simple LED SM
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
//#include “sm.h”


enum States {Init, PressOn, Stay, PressOff} state;
//press2, release2
//Global variables here
#define A0 (PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


void tick()
{
	switch (state) { //Transitions
		/*case Start:
			state = Init;
		break;*/
		
		case Init:
		//Check if button has been pressed
			if(A0)
			{ state = PressOn; }//pressed, go to next state 
			else {state = Init; }//not pressed, stay in Init
		break;
			
		case PressOn: //turn on led
			if(A0)
			{ state = PressOn; } //next state
			else {state = Stay; }//still pressed stay 
		break;
			
		case Stay:
		//Check if button has been released
			if(A0)
			{ state = PressOff; }//not pressed, LED stays on
			else {state = Stay; }//pressed, go to next state
		break;
		
		case PressOff:
		//Check if button has been pressed
			if(A0)
			{ state = PressOff; }//still pressed, stay in PressOff
			else {state = Init;}//pressed, go to back to Init 
		break;
			
		default:
		break;
	}
	
	switch (state) { //State Actions
		case Start:
		break;
				
		case Init:
		//Set PB0 LED to On initially
			PORTB = SetBit(PORTB, 0, 1);
		break;

		case PressOn: 
		//Set PB1 LED On & PB0 Off
			PORTB = SetBit(PORTB, 0, 0);
			PORTB = SetBit(PORTB, 1, 1);
		break;

		case Stay:
		//Leave PB1 LED On 
			PORTB = SetBit(PORTB, 0, 0);
			PORTB = SetBit(PORTB, 1, 1);
		break;

		case PressOff:
		//Turn PB1 LED Off and Set PB0 On
			PORTB = SetBit(PORTB, 1, 0);
			PORTB = SetBit(PORTB, 0, 1);
		break;

		default:
		break;	
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	//state = Start;//initialize state
	state = Init;
	while(1) { tick(); }
}