 /* 
 * Name & Email: Itzel G. igonz044@ucr.edu
 * Lab Section: 026
 * Assignment: Final Project: Complexity 1 - Joystick
 * Exercise Description: Read README file 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

#include <avr/io.h>
 //header to enable data flow control over pins
 #define F_CPU 1000000
 //telling controller crystal frequency attached
 #include <util/delay.h>
 //header to enable delay function in program
 #define    E   5
 //giving name “enable”  to 5th pin of PORTD, since it Is connected to LCD enable pin
 #define RS  6
 //giving name “register selection” to 6th pin of PORTD, since is connected to LCD RS pin
 void send_a_command(unsigned char command);
 void send_a_character(unsigned char character);
 void send_a_string(char *string_of_characters);

 int main(void)
 {
	 DDRB = 0xFF; PORTB = 0x00;
	 //putting portB and portD as output pins
	 DDRD = 0xFF; PORTD = 0x00;
	 _delay_ms(50);//giving delay of 50ms
	 DDRA = 0x00; PORTA = 0xFF;//Joystick input.
	 
	 int StartH = 520;//neutral value on x-axis
	 int StartV = 520;// neutral value on y-axis
	 int MoveH =0;
	 int MoveV =0;
	
	 ADMUX |=(1<<REFS0);//setting the reference of ADC
	 
	 ADCSRA |=(1<<ADEN) |(1<ADPS2)|(1<ADPS1) |(1<<ADPS0);
	 //enabling the ADC,, setting prescalar 128
	 
	 while(1)
	 {
		 switch (ADMUX)//changing between channels by switch statement
		 {
			 case 0x40://When ADMUX==0x40
			 {
				 ADCSRA |=(1<<ADSC);//start ADC conversion
				 while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
				 MoveH = ADC;//moving value
				 ADC=0;//reset ADC register
				 ADMUX=0x41;//changing channel
				 break;
			 }

			 case 0x41:
			 {
				 ADCSRA |=(1<<ADSC);// start ADC conversion
				 while ( !(ADCSRA & (1<<ADIF)));// wait till ADC conversion
				 MoveV = ADC;// moving value
				 ADC=0;// reset ADC register
				 ADMUX=0x40;// changing channel
				 break;
			 }
		 }
		 ///////////////////////////////////////////////////////////////////////////////////
		 if (MoveH > StartH + 500) //changed this value from the original code
		 {
			 PORTD |=(1<<PIND1);//right
			 _delay_ms(5);
			 PORTD=0;
		 }

		 if (MoveH < (StartH)) //changed this value from the original code
		 {
			 PORTD |=(1<<PIND2);//left 
			 _delay_ms(5);
			 PORTD=0;
		 }

		 if (MoveV < StartV) //changed this value from the original code
		 {
			 PORTD |=(1<<PIND0);//up
			 _delay_ms(5);
			 PORTD=0;
		 }
		 
		 if (MoveV > StartV+500) //changed this value from the original code
		 {
			 PORTD |=(1<<PIND3);//down
			 _delay_ms(5);
			 PORTD=0;
		 }
	 }
 }
