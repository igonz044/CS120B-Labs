/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab1_part3.c
 * Created: 4/7/2019 11:57:13 AM
 * Author : ucrcse
 * Exercise Description: Parking spaces modified
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>

int main(void)
{
	//DDR - registers
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned char spaces = 4;
	
	/* Replace with your application code */
	while (1)
	{
		unsigned char cntavail = spaces;
		unsigned char i = 0;
		while(spaces > i)
		{
			if(PINA & (0x01 << i))
			{
				cntavail--;
			}//end of if
			i++;
		}//end of while
		
		if (cntavail == 0) { PORTC = cntavail | 0x80; } 
		else               { PORTC = cntavail; }
	}
	return 0;
}

