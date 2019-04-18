/*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab4_part3.c 
 * Created: 4/12/2019 10:36:24 AM
 * Author : ucrcse
 * Exercise Description: Security Vault, there is a key pad ordered #yx. 
 * The sequence # followed by y unlocks vault and turns on PB0 LED
 * To lock it back up from inside you press A7
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 
#include <avr/io.h>
enum States {Locked, Wait, Unlocked} state;

//Global variables here

#define A0 (PINA & 0x01)
#define A1 (PINA & 0x02)
#define A2 (PINA & 0x04)
#define A7 (PINA & 0x80)
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
		case Locked:
			if(A2 && !A0 && !A1)
			{
				state = Wait;
			}
			else
			{
				state = Locked;
			}
		break;
		
		case Wait:
			if(A1 && !A0 && !A2)
			{
				state = Unlocked;
			}
			else if(A7)
			{
				state = Locked;
			}
			else
			{
				state = Wait;
			}
		break;
		
		case Unlocked://opens vault
		if(A7)// to lock it back up
		{
			state = Locked;
		}
		else
		{
			state = Unlocked;
		}
		break;

		default: 
		break;
	}
	//////////////////////////////////////////
	switch (state) { //State Actions
		case Locked:
			PORTB = 0x00;
		break;
		
		case Wait:
			PORTB = 0x00;
		break;
		
		case Unlocked://opens vault
			PORTB = 0x01;
		break;

		default: 
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs, 2 buttons
	DDRB = 0xFF; PORTB = 0x00; //outputs
	
	state = Locked;//initialize state
	
	while(1) {tick();}
	return 0;
}
