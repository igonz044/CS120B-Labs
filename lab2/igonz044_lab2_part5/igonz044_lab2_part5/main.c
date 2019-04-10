 /* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part5.c
 * Created: 4/10/2019 2:03:15 AM
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
	DDRD = 0x00; PORTD = 0xFF;//input
	DDRB = 0xFE; PORTB = 0x01;//input on high nibble & output on low nibble
	
	unsigned char tmpA = 0x00;
	
	
    /* Replace with your application code */
    while (1) 
    {
		tmpA = PIND;
		tmpA = tmpA << 1;
		tmpA = tmpA | (PINB & 0x01);

		if (tmpA > 70){
			PORTB = SetBit(PORTB, 1, 1);
			PORTB = SetBit(PORTB, 2, 0);
		} 
		else if (tmpA > 5){
			PORTB = SetBit(PORTB, 1, 0);
			PORTB = SetBit(PORTB, 2, 1);
		} 
		else {
			PORTB = SetBit(PORTB, 1, 0);
			PORTB = SetBit(PORTB, 2, 0);
		}

		if (tmpA == 511) {PORTB = SetBit(PORTB, 7, 0);}
    }
}

