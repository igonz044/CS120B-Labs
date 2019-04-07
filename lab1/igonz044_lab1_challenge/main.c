 /*
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab1_challenge.c 
 * Created: 4/7/2019 12:25:50 PM
 * Author : ucrcse
 * Exercise Description: Garage opener
 */ 
#include <avr/io.h>

int main(void)
{
    /*AMUSEMENT PARK RIDE*/
	//weight sensors weight 0 - 255 
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char seatA = PINA;
	unsigned char seatB = PINB;
	unsigned char seatC = PINC;
	unsigned char weight_sensor = seatA + seatB + seatC;
	
    while (1) 
    {
		if(weight_sensor > 140)
		{
			PORTA = 0x01;
		}
		else if(seatA - seatC > 80)
		{
			PORTB = 0x01;
		}
    }
}

