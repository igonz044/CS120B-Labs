/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part1.c
 * Created: 4/9/2019 4:43:37 PM
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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char size = 8;
	//unsigned char tmp = 0x00;
	
	//unsigned char tmp = ;

	//unsigned char is_bit_1_or_0 = 0;
	
	while(1)
	{
		unsigned char tmp = PINA;
		unsigned char tmp2 = PINB;
		unsigned char count = 0;		
		for(unsigned char i = 0; i < size; i++)
		{
			//tmp = PINA;
			//is_bit_1_or_0 = GetBit(tmp, i);
			if(GetBit(tmp, i))
			{
				count++;
			}
			if(GetBit(tmp2, i))
			{
				count++;
			}
		}
		PORTC = count;
	}
}
/*unsigned char GetBits(unsigned char tmp, unsigned char size) 
{
	unsigned char count = 0;
	for (unsigned char i = 0; i < size; i++) 
	{
		if (tmp & (0x01 << i)) {count++;}
	}
	return count;
}*/