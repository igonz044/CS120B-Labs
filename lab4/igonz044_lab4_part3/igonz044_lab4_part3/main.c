/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab4_part3.c 
 * Created: 4/12/2019 10:36:24 AM
 * Author : ucrcse
 * Exercise Description: Security System
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 
#include <avr/io.h>
enum States {Locked, Wait, Unlocked} state;

//Global variables here
/*
#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x04)

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}
*/
void tick()
{
	switch(state)//Transitions
	{
		case Locked:
			state = wait;
		break;
		
		case Wait:
			if(!(PINA && 0x04)) { state = entry2;}
			else   {state = wait;}
		break;
		
		case Unlocked://opens vault
		break;

		default: 
		break;
	}	
	switch (state) { //State Actions
		case Locked:
			state = wait;
		break;
		
		case Wait:
			if(!(PINA && 0x04)) { state = entry2;}
			else   {state = wait;}
		break;
		
		case Unlocked://opens vault
		break;

		default: 
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs, 2 buttons
	DDRB = 0xFF; PORTB = 0x00; //outputs
	
	state = Start;//initialize state
	
	while(1) { 

		tick();
	}
	return 0;
}
