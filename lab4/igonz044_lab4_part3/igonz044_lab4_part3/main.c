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
enum States {Start, locked, entry1, wait, entry2, unlocked}state;

//Global variables here
#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)
#define A2 (~PINA & 0x03)

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k){
	return ((x & (0x01 << k)) != 0);
}

void tick()
{
	switch(state)//Transitions
	{
		case Start:
			state = locked;
		break;
		
		case locked:
			state = wait;
		break;
		
		case entry1:
			if(A2){ state = wait;}
			else  { state = locked;}
		break;
		
		case wait:
			if(A2) { state = wait;}
			else   {state = wait;}
		break;
		
		case entry2:
			if(A1){ state = unlocked;}
			else  { state = locked;}
		break;
		
		case unlocked://opens vault
		break;

		default: break;
	}	
	switch (state) { //State Actions
		case Start:
		//state = locked;
		break;
		
		case locked:
		break;
		
		case entry1:
		break;
		
		case wait:
		break;
		
		case entry2:
		break;
		
		case unlocked:
		PORTB = 0x01;
