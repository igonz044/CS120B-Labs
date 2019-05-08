/* itzel_lab9_part2.c
 *
 * Created: 5/6/2019 11:08:10 PM
 * Author : shado
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

#define A0 ((~PINA)&0x01)
#define A1 ((~PINA)&0x02)
#define A2 ((~PINA)&0x04)

enum States{Start, Off, On} state;
enum States2{Init, Base, Incr, Rel1, Decr, Rel2} state2;

unsigned char toggle = 0;
unsigned char index = 0;
unsigned char SIZE = 8;

double freq[8] = {261.63, 293.66, 329.63, 349.23,
				  392.00, 440.00, 493.88, 523.25};
void ToggleOnOff();
void ChangeNotes();

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR3A from underflowing, using prescaler 64					
		else if (frequency > 31250) { OCR3A = 0x0000; }// 31250 is largest frequency that will not result in underflow
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR3A = (1 << COM0A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM02) | (1 << CS01) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

/*//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

const unsigned short timerPeriod = 500;
volatile unsigned char TimerFlag = 0;
void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}
void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}
void TimerISR() {TimerFlag = 1;}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}*/

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs
	DDRB = 0xFF; PORTB = 0x00; //outputs
    
	state = Start;
	state2 = Base;
	
	/*TimerSet(timerPeriod);
	TimerOn();*/
	PWM_on();
    while (1){
		ToggleOnOff();
		ChangeNotes();
		set_PWM(freq[index]);
		/*while(!TimerFlag){}
		TimerFlag = 0;*/
	}
}

void ToggleOnOff()
{
	switch(state)//State Transitions 
	{
		case Start:
		state = Off;
		break;
		
		case Off:
		if(A0 == 0x01){state = On;}
		else {state = Off;}
		break;
		
		case On:
		if(A0 == 0x01){state = Off;}
		else {state = On;}
		break;
		
		default:
		state = Off;
		break;
	}
	switch(state)//State Actions
	{
		case Start:
		break;
		
		case Off:
		toggle = 0;
		break;
		
		case On:
		toggle = 1;
		break;
		
		default:
		break;
	}
}
void ChangeNotes()
{
	switch(state2)//State Transitions
	{
		case Init:
			state2 = Base;
			break;
			
		case Base:
			if(A1 && toggle == 1 && index <= 7){state2 = Incr; }
			else{state2 = Base; }
			break;
		
		case Incr:
			state2 = Rel1;
			break;
		
		case Rel1:
			if(A1 && toggle == 1 && index <= 7){state2 = Incr; }
			else if(A2 && toggle == 1 && index >= 0){state2 = Decr; }
			else{state2 = Rel1; }
			break;
		
		case Decr:
			state2 = Rel2;
			break;
		
		case Rel2:
			if(A1 && toggle == 1 && index <= 7){state2 = Incr; }
			else if(A2 && toggle == 1 && index >= 0){state2 = Decr; }
			else{state2 = Base; }
			break;
	}//State Transitions
	
	switch(state2)//State Actions
	{
		case Init:
			index = 0;
			PWM_off();
			break;
		
		case Base:
			PWM_on();
			set_PWM(0);
			break;
		
		case Incr:
			if(index <=7){index++; set_PWM(freq[index]);}
			else{index = 8;}
			break;
		
		case Rel1:
		
			break;
		
		case Decr:
			if(index >=0){index--; set_PWM(freq[index]);}
			else{index = 0;}
			break;
		
		case Rel2:
		
			break;
	}//State Actions
}