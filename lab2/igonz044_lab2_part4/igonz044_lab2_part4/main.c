/*
 * igonz044_lab2_part4.c
 *
 * Created: 4/10/2019 1:07:12 AM
 * Author : ucrcse
 */ 
/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part4.c
 * Created: 4/10/2019 1:07:12 AM
 * Author : ucrcse
 * Exercise Description: Switching nibbles around
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>

//Bit access functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char botNibl(unsigned char num){return (0x0F & num);}
unsigned char topNibl(unsigned char num){return (0xF0 & num);}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;//input
	DDRB = 0x0F; PORTB = 0x00;//input on high nibble & output on low nibble
	DDRC = 0xF0; PORTC = 0x00;
	
	//DDRB = 0x0F; PORTB = 0x00;//input on high nibble & output on low nibble
	//DDRC = 0xF0; PORTC = 0x00;//output on high nibble & input on low nibble
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB= 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		//Step 1: Shift input PINA 
		tmpA = PINA>>4; //making upper 0
		tmpB = PINA<<4; //making lower 0
		//Step 2: Mask lower nibble of PortB
		PORTB = topNibl(PORTB) + tmpA;
		PORTC = tmpB + botNibl(PORTC);
		
		//Step 3: Add these together and set to PORTB ..Nevermind!
		/*PORTB = tmpA + tmpB1; //If PINA */
    }
}

