/*
 * igonz044_lab1_part1.c
 *
 * Created: 4/3/2019 10:26:24 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>
{ 
	//DDR - registers 
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs 
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s 
	
	unsigned char garage_sensor = 0x00; // Temporary variable to hold the value of B 
	unsigned char light_sensor = 0x00; // Temporary variable to hold the value of A 
	unsigned char LED = 0x00; 
	
	while(1) 
	{ 
		// 1) Read input 
		garage_sensor = PINA & 0x01; 
		light_sensor = PINA & 0x02; 
		LED = PINB & 0x00; 
		
		// 2) Perform computation
		// if PA0 is 1, set PB1PB0 = 01, else = 10 
		if (garage_sensor == 0x01 && light_sensor == 0x00){ 
			//True if PA0 is 1 
			LED = (LED & 0xFC) | 0x01; // Sets tmpB to bbbbbb01 
			//(clear rightmost 2 bits, then set to 01) 
		} 
		else{ 
			LED = (LED & 0xFC) | 0x00; // Sets tmpB to bbbbbb01 
			// (clear rightmost 2 bits, then set to 10)
		}
		
		// 3) Write output
		PORTB = LED;
	} 
	return 0; 
}

