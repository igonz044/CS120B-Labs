/* My Name and email : Jimmy Le: jle071@ucr.edu
* Partner Name and email: Reyna Lopez rlope071@ucr.edu
 
* Lab Section: 26
* Assignment: Lab 4 Exercise 3
* Exercise Description: In this exercise, we have a house that has a digital combination deadbolt lock system on its door. 
* Buttons A0, A1, and A2 are X, Y, and # respectively. Pressing # and then Y will open the door and set B0 to 1. Any other 
* combination will reset the code. A7 is a button from the inside of the house and pressing it will lock the door, setting
* B0 to 0. For debugging purposes, give each state a number, and always write the current state to PORTC 
* (consider using the enum state variable). Also, be sure to check that only one button is pressed at a time.

 
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.

 

#include <avr/io.h>

enum States{ Init, Locked, WaitRel, PressPound, PressY, Unlock} State;


void tick()
{
	switch (State)
	{
		case Init:
			State = Locked;
			break;

		case Locked:
			if(PINA == 0x04)
			{
				State = WaitRel;
			}
			else 
			{
				State = Locked;
			}
			break;

		case WaitRel:
			if(PINA == 0x04)
			{
				State = WaitRel;
			}
			else if(PINA == 0x00)
			{
				State = PressPound;
			}
			break;

		case PressPound:
			if(PINA == 0x02)
			{
				State = PressY;
			}
			else if (PINA == 0x00)
			{
			State = PressPound;
			}
			else
			{
				State = Locked;	
			}
			break;

		case PressY:
			if(PINA == 0x02)
			{
				State = Unlock;
			}
			else
			{
				State = Locked;
			}
			break;

		case Unlock:
			if(PINA == 0x80)
			{
				State = Locked;
			}
			break;

		default:
			State = Init;
			break;
	}
	switch(State)
	{
		case Init:
			PORTC=Init;
			PORTB = 0x00;
			break;

		case Locked:
			PORTC=Locked;
			PORTB=0x00;
			break;

		case WaitRel:
			PORTC=WaitRel;
			break;

		case PressPound:
			PORTC=PressPound;
			break;

		case PressY:
			PORTC=PressY;
			break;

		case Unlock:
			PORTC=Unlock;
			PORTB=0x01;
			break;

		default:
			PORTB=0x00;
			break;
	}
}
int main(void)
{
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	DDRC=0xFF; PORTC=0x00; 
	PORTC = Init;
	State = Init;
	while(1)
	{
		tick();
	}
	return 0;
}*/

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
		else if(!A0 && A1)
		{state = Dec;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Init;}
        break;
        
        case Dec:
		if(A0 && !A1)
		{state = Inc;}
		else if(!A0 && A1)
		{state = Wait2;}
		else if(A0 && A1)
		{state = Reset;}
		else
		{ state = Init;}
	break;
        
        case Wait://for inc
		if(A0 && !A1)
		{state = Inc;}
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
		else if(!A0 && A1)
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

        case Wait://do not change portB
	PORTB = PORTB;
	break;
		
	case Wait2://do not change portB
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



