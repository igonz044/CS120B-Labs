#include <avr/io.h>
#include <avr/interrupt.h>
//#include "io.c"

enum BL_States {BL_start, off, on} BL_state;
enum TL_States {TL_start, l1, l2, l3} TL_state;
	
unsigned char BL_next;
unsigned char TL_next;
unsigned char outputB = 0x00;
unsigned char outputT = 0x00;

void LED_TL();
void LED_BL();

/*
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
*/
//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

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
}              

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; //inputs
	DDRB = 0xFF; PORTB = 0x00; //outputs
	
	unsigned long BL_timer = 1000;
	unsigned long TL_timer = 300;
	const unsigned long BL_limit = 1000;
	const unsigned long TL_limit = 300;
	
	const unsigned long timer = 100;
	
	TimerSet(timer);
	TimerOn();
    
	while (1){
		PORTB = outputT << 1 | (outputB);
		
		if(TL_timer >= TL_limit)
		{
			LED_TL();
			TL_timer = 0;
		}
		if(BL_timer >= BL_limit)
		{
			LED_BL();
			BL_timer = 0;
		}
		
		while(!TimerFlag){}
		TimerFlag = 0;
		
		BL_timer += timer;
		TL_timer += timer;
	}
}

void LED_BL()
{
	switch(BL_state){//State Transitions
		case BL_start:
		BL_next = on;
		break;
		
		case on:
		BL_next = off;
		break;
		
		case off:
		BL_next = on;
		break;
		
		default:
		BL_next = BL_start;
		break;
	}//State Transitions
	switch(BL_state){//State Actions
		case BL_start:
		outputB = 0x00;
		break;
		
		case on:
		outputB = 0x01;
		break;
		
		case off:
		outputB = 0x00;
		break;
		
		default:
		break;
	}//State Actions
	BL_state = BL_next;
}

void LED_TL()
{
	switch(TL_state){//State Transitions
		case TL_start:
		TL_next = l1;
		break;
		
		case l1:
		TL_next = l2;
		break;
		
		case l2:
		TL_next = l3;
		break;
		
		case l3:
		TL_next = l1;
		break;
		
		default:
		break;
	}//State Transitions
	switch(TL_state){//State Transitions
		case TL_start:
		outputT = 0x00;
		break;
		
		case l1:
		outputT = 0x01;
		break;
		
		case l2:
		outputT = 0x02;
		break;
		
		case l3:
		outputT = 0x04;
		break;
		
		default:
		break;
	}//State Actions
	TL_state = TL_next;
}
