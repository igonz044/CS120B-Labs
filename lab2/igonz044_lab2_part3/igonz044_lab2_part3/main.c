/* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: igonz044_lab2_part3.c
 * Created: 4/9/2019 10:32:36 PM
 * Author : ucrcse
 * Exercise Description: Displays: Fasten Seatbelt icon, Low fuel icon, and how full your car gas tank is!
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
unsigned char botNibl(unsigned char num)
{
	return (0x0F & num);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	//DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmp = 0x00;
	while(1)
	{
		tmp = PINA;
		
		if ( botNibl(tmp) >= 1 && (botNibl(tmp))<= 2)//low fuel, 1-2
		{
			PORTC = 0x60;
		} 
		if (botNibl(tmp) >= 3 && botNibl(tmp) <= 4)//3 to 4
		{
			PORTC = 0x70; 
		} 
		if (botNibl(tmp) >= 5 && botNibl(tmp) <= 6)//5 to 6
		{
			PORTC = 0x38; 
		} 
		if (botNibl(tmp) >= 7 && botNibl(tmp) <= 9)//7 to 9 
		{
			PORTC = 0x3C;  
		} 
		if(botNibl(tmp) >= 10 && botNibl(tmp) <= 12)//10 to 12 
		{
			PORTC = 0x3E; 
		} 
		if (botNibl(tmp) >= 13 && botNibl(tmp) <= 15)//13 to 15 
		{
			PORTC = 0x3F; 
		}
		if(botNibl(tmp) <= 4)//low fuel icon connected to PC6 is displayed
		{
			//PORTC = 0x40;
			PORTC = SetBit(PORTC, 6, 1);
		}
		////////////////////////////////////////////////////////////////////
		//Fasten Seat belt
		 if (GetBit(tmp, 4) && GetBit(tmp, 5) && !GetBit(tmp, 6))//Fasten seatbelt warning
		{
			PORTC = (PORTC | 0x80);
		}
		
		/*else{//cehcking if it works
			PORTC = 0xC3; //cool pattern
		}
		if (tmp == 0x10)//key is in ignition?
		{
			PORTC = SetBit(tmp, 4,1);
		}
		if (tmp == 0x20)//driver is seated?
		{
			PORTC = SetBit(tmp, 5,1);
		}
		if (tmp == 0x40)//seat belt is fastened?
		{
			PORTC = SetBit(tmp, 6,1);
		}
		if (tmp == 0x30)//Fasten seatbelt warning
		{
			PORTC = 0x80;
		}
		
		
		if (GetBit(tmp, 4))//key is in ignition?
		{
			//SetBit(tmp, 4,1);
		}
		if (GetBit(tmp, 5))//driver is seated?
		{
			//PORTC = SetBit(tmp, 5,1);
		}
		if (GetBit(tmp, 6))//seat belt is fastened?
		{
			//PORTC = SetBit(tmp, 6,1);
		}*/
	}
}

