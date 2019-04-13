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

unsigned char Increment(unsigned char x){ return x += 1;}
unsigned char Decrement(unsigned char x){ return x -= 1;}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
    return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k){
    return ((x & (0x01 << k)) != 0);
}

enum States {Start, Init, Inc, Dec, Reset, Wait1, Wait2} state;

//Global variables here
#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)

void tick()
{
	switch (state) { //Transitions
		case Start:
			state = Init;
        break;  
		
		case Init:
		if(A0 && !A1){state = Inc;}
        else if(!A0 && A1){state = Dec;}
		else if(!A0 && !A1){state = Init;}
        else if(A0 && A1){state = Reset;}
        else{state = Init;}
        break;

        case Inc:
          state = Wait1; 
        break;
        
        case Dec:
           state = Wait2; 
        break;
        
        case Wait1://check if buttons have been released
            //not pressed, count stays the same
        if (A0 && A1){state = Reset;}
        if (A0)      {state = Wait1;}
        if (!A0)     {state = Init; }
        break;

        case Wait2:
        if (A0 && A1){state = Reset;}
        if (A1)      {state = Wait2;}
        else if (!A1){state = Init; }
        
        case Reset:
            state = Init; 
        break;

		default: 
		break;
	}
	
	switch (state) { //State Actions
		case Start:
		    PORTB = 0x07; 
        break;
				
		case Init:
			break;

		case Inc:
            if(PORTB<9)
             PORTB= Increment(PORTB); break;
		
		case Dec:
             if(PORTB>0)
               PORTB =Decrement(PORTB) ; break;
case Wait1: break;
case Wait2: break;
		
		case Reset:
		    PORTB = 0x07; break;

		default:
		break;	
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs, 2 buttons
	DDRB = 0xFF; PORTB = 0x00; //outputs 
	PORTB = 0x07;
	state = Start;//initialize state

	
 	while(1) { tick(); }
}