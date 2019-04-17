/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab4_challenge1.c 
 * Created: 4/12/2019 10:55:34 AM
 * Author : ucrcse
 * Exercise Description: Security System: Lock
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */  
#include <avr/io.h>
enum States {Unlocked, Wait, Locked} state;

//Global variables here
#define A0 (PINA & 0x01)
#define A1 (PINA & 0x02)
#define A2 (PINA & 0x04)
/*
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
		case Unlocked:
			if(A2&0x04 && !A0 && !A1)
			{
				state = Wait;
			}
			else
			{
				state = Unlocked;
			}
		break;
		
		case Wait:
			if(A1&0x02 && !A0 && !A2)
			{
				state = Locked;
			}
			else
			{
				state = Wait;
			}
		break;
		
		case Locked://opens vault
		break;

		default: 
		break;
	}	
	switch (state) { //State Actions
		case Unlocked:
			PORTB = 0x01;
		break;
		
		case Wait:
			PORTB = 0x01;
		break;
		
		case Locked://opens vault
			PORTB = 0x00;
		break;

		default: 
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs, 2 buttons
	DDRB = 0xFF; PORTB = 0x00; //outputs
	
	state = Unlocked;//initialize state
	
	while(1) { tick(); }
	return 0;
}