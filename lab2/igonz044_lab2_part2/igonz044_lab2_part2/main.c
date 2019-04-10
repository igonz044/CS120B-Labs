/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part2.c
 * Created: 4/9/2019 9:13:02 PM
 * Author : ucrcse
 * Exercise Description: Parking spaces modified
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

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	//DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	while(1)
	{
		unsigned char tmp = PINA;

//if(tmp >= 1 && tmp <= 2)
		if(tmp == 0)//low fuel icon connected to PC6 is displayed
		{
			PORTC = 0x40;
		}
		if (tmp >= 1 && tmp <= 2)//low fuel, 1-2
		{
			PORTC = 0x60;
		} 
		else if (tmp >= 3 && tmp <= 4)//3 to 4
		{
			PORTC = 0x70; 
		} 
		else if (tmp >= 5 && tmp <= 6)//5 to 6
		{
			PORTC = 0x38; 
		} 
		else if (tmp >= 7 && tmp <= 9)//7 to 9 
		{
			PORTC = 0x3C;  
		} 
		else if (tmp >= 10 && tmp <= 12)//10 to 12 
		{
			PORTC = 0x3E; 
		} 
		else if (tmp >= 13 && tmp <= 15)//13 to 15 
		{
			PORTC = 0x3F; 
		} 
		else {
			PORTC = 0x00;
		}
	//
	}
}

