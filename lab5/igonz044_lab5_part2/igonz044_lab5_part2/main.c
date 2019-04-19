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

enum States {Init, Inc, Dec, Reset, Wait, Wait2} state;

//Global variables here

#define A0 (PINA & 0x01)
#define A1 (PINA & 0x02)

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
		if(A0 && !A1)
		{state = Inc;}
		else if(
		if(!A0 && A1)
		{state = Dec;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Init;}
		break;
        
        case Inc:
		if(A0 && !A1)
		{state = Wait;}
		else if(
		if(!A0 && A1)
		{state = Dec;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Init;}
        break;
        
        case Dec:
		if(A0 && !A1)
		{state = Inc;}
		else if(
		if(!A0 && A1)
		{state = Wait2;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Init;}
	break;
        
        case Wait://for inc
		if(A0 && !A1)
		{state = Inc;}
		else if(
		if(!A0 && A1)
		{state = Dec;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Wait;}
        break;
		
	case Wait2: //for dec 
		if(A0 && !A1)
		{state = Inc;}
		else if(
		if(!A0 && A1)
		{state = Dec;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Wait2;}
        break;
	
        case Reset:
		if(A0 && !A1)
		{state = Inc;}
		else if(
		if(!A0 && A1)
		{state = Dec;}
		else
		{ state = Reset;}
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
		
	case Stay2://do not change portB
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
