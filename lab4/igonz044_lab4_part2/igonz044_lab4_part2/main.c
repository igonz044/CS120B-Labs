/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab4_part2.c
 * Created: 4/12/2019 6:25:06 PM
 * Author : ucrcse
 * Exercise Description: Simple LED SM
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
//#include “sm.h”


enum States {bla, Start, Init, Inc, Dec, Reset, Stay} state;

//Global variables here

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
		case bla:
			state = Start;
		break;
		
		case Start:
			state = Init;
		break;
		
		case Init:
		//In this state you can dec, incr, or reset from here
			if(GetBit(PINA, 0) && PORTB <= 0x09){//if incrementing
    			state = Inc;//increment
			}
			else if (GetBit(PINA, 1) && PORTB >= 0x00){
    			state = Dec; //decrement
			}
			else if (GetBit(PINA, 0)&&GetBit(PINA, 1)){//both pressed together
    			state = Reset;
			}
			else if(!GetBit(PINA, 0) && !GetBit(PINA, 1)){ state = Init; }
		break;
        
        case Inc:
            if(GetBit(PINA, 0) && PORTB <= 0x09){//if incrementing
                state = Stay;//increment
            }
            else if (GetBit(PINA, 1) && PORTB >= 0x00){
                state = Dec; //decrement
            }
            else if (GetBit(PINA, 0) && GetBit(PINA, 1)){//both pressed together
                state = Reset;
            }
            else if(!GetBit(PINA, 0) && !GetBit(PINA, 1)){ state = Init; }
        break;
        
        case Dec:
            if(GetBit(PINA, 0) && PORTB <= 0x09){//if incrementing
                state = Inc;//increment
            }
            else if (GetBit(PINA, 1) && PORTB >= 0x00){
                state = Stay; //decrement
            }
            else if (GetBit(PINA, 0) && GetBit(PINA, 1)){//both pressed together
                state = Reset;
            }
            else if(!GetBit(PINA, 0) && !GetBit(PINA, 1)){ state = Init; }
        break;
        
        case Stay://check if buttons have been released
            //not pressed, count stays the same
            if(GetBit(PINA, 0) && PORTB <= 0x09){//if incrementing
                state = Stay; 
            }
            if (GetBit(PINA, 1) && PORTB >= 0x00){//decrement
                state = Stay; 
            }
            if (GetBit(PINA, 0) && GetBit(PINA, 1)){//both pressed together
                state = Reset;
            }
            else if(!GetBit(PINA, 0) && !GetBit(PINA, 1)){ state = Init; }
        break;
        
        case Reset:
            if(GetBit(PINA, 0) && PORTB <= 0x09){//if incrementing
                state = Inc;//increment
            }
            else if (GetBit(PINA, 1) && PORTB >= 0x00){
                state = Dec; //decrement
            }
            else if (GetBit(PINA, 0) && GetBit(PINA, 1)){//both pressed together
                state = Reset;
            }
			else if(!GetBit(PINA, 0) && !GetBit(PINA, 1)){ state = Init; }
        break;

		default:
		break;
	}
	
	switch (state) { //State Actions
		case Start:
		PORTB = 0x07; break;
				
		case Init:
			break;

		case Inc:
            PORTB = Increment(PORTB); break;
		
		case Dec:
            PORTB = Decrement(PORTB); break;		

        case Stay://do not change portB
            PORTB = PORTB; break;

		case Reset:
		    PORTB = 0x00; break;

		default:
		break;	
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs, 2 buttons
	DDRB = 0xFF; PORTB = 0x00; //outputs 
	
	state = bla;//initialize state
	
	while(1) { tick();}
}
