/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab5_part2.c
 * Created: 4/17/2019 3:28:53 PM
 * Author : ucrcse
 * Exercise Description: Simple LED SM
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
//#include “sm.h”


enum States {Init, Inc, Dec, Reset, Stay, Stay2} state;

//Global variables here
#define I (PINA & 0x01)//xxxx 0001
#define D (PINA & 0x02)//xxxx 0010
//#define R (GetBit(PINA, 0) && GetBit(PINA, 1))//xxxx 0011

unsigned char Increment(unsigned char x){ return x += 1;}
unsigned char Decrement(unsigned char x){ return x -= 1;}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}


void tick()
{
 	switch (state) { //Transitions
		case Init:
		//In this state you can dec, incr, or reset from here
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
				state = Inc;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Dec;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Init;
			}
		break;
        
        case Inc:
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
				state = Stay;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Dec;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Stay;
			}
        break;
        
        case Dec:
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
				state = Inc;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Stay2;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Stay2;
			}
        break;
        
        case Stay:
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
				state = Stay;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Dec;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Stay;
			}
        break;
		
		case Stay2:
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
				state = Inc;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Stay2;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Stay2;
			}
        break;
	
        case Reset:
			if(GetBit(PINA, 0) && !GetBit(PINA, 1) && PORTB <= 0x09)
			{
 				state = Inc;
			}
			else if(GetBit(PINA, 1) && !GetBit(PINA, 0) && PORTB >= 0x00)
			{
				state = Dec;
			}
			else if(GetBit(PINA, 0) && GetBit(PINA, 1))
			{
				state = Reset;
			}
			else
			{
				state = Reset;
			}
        break;

		default:
		break;
	}
	//////////////////////////////////////////////////////
	switch (state) { //State Actions		
		case Init:
			PORTB = 0x07;
			break;

		case Inc:
            PORTB = PORTB+1; 
			break;
		
		case Dec:
            PORTB = PORTB-1; 
			break;

        case Stay://do not change portB
			PORTB = PORTB;
			break;

		case Reset:
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
	
	state = Init;//initialize state
	
	while(1) { tick();}
}