 /* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part5.c
 * Created: 4/10/2019 2:03:15 AM
 * Author : ucrcse
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
	DDRD = 0x00; PORTD = 0xFF;//input
	DDRB = 0xFE; PORTB = 0x01;//input on high nibble & output on low nibble
	
	unsigned char tmpD = 0x00;
	unsigned char tmpB = 0x00;
	
	
    /* Replace with your application code */
    while (1) 
    {
	        tmpD = PIND;
	        tmpB = PINB&0x01;

		if (tmpD >= 70 || (tmpD >= 69 && tmpB == 1)){
			PORTB = SetBit(PORTB, 1, 1);
		} 
		else if (tmpD >= 5 || (tmpD >= 4 && tmpB == 1)){
			PORTB = SetBit(PORTB, 3, 1);
		}
	    	else{ PORTB = 0x00; }
    }
}

