/*
 * igonz044_lab10_part5.c
 *
 * Created: 5/13/2019 2:07:42 AM
 * Author : ucrcse
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;
// TimerISR() sets this to 1. C programmer should clear to 0.

enum States {Start, Init, Wait1, Incr, Wait2, Decr, Reset} state;

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

unsigned char PORTCout;
unsigned char PINAtmp;
unsigned char next_state;
unsigned char counter;
unsigned char TEMP = 0;
unsigned char TEMP2 = 10;

void IncrDecr()
{
	switch(state){
		
		case Start:
		next_state = Init;
		break;
		
		case Init:
		if(PINAtmp==1){
			next_state = Wait1;
		}
		else if (PINAtmp==2){
			next_state = Wait2;
		}
		else if(PINAtmp ==3){
			next_state = Reset;
		}
		else{
			next_state = Init;
		}
		break;
		
		case Wait1:
		if(PINAtmp==1){
			next_state = Wait1;
		}
		else if(PINAtmp==3){
			next_state = Reset;
		}
		else{
			next_state = Incr;
		}
		break;
		
		case Wait2:
		if(PINAtmp==2){
			next_state = Wait2;
		}
		else if(PINAtmp==3){
			next_state = Reset;
		}
		else{
			next_state = Decr;
		}
		break;
		
		case Incr:
		next_state = Init;
		break;
		
		case Decr:
		next_state = Init;
		break;
		
		case Reset:
		next_state = Init;
		break;
		
		default:
		next_state = Start;
		break;
	}
	
	
	switch(state){
		case Start:
		PORTCout = 0;
		counter = 0;
		break;
		
		case Init:
		PORTCout = PORTCout;
		break;
		
		case Wait1:
		if(counter >= TEMP2){
			PORTCout = (PORTCout>= 9) ? PORTCout: PORTCout+1;
			counter = 0; TEMP++;
			if(TEMP >= 3) {TEMP2 = 4;}
		}
		else{
			counter++;
		}
		break;
		
		
		case Incr:
		PORTCout = (PORTCout>= 9) ? PORTCout: PORTCout+1;
		TEMP2 = 10;
		TEMP= 0;
		break;
		
		
		case Wait2:
		if(counter >= TEMP2){
			PORTCout = (PORTCout<= 0) ? PORTCout: PORTCout-1;
			counter = 0; TEMP++;
			if(TEMP >= 3) {TEMP2 = 4;}
		}
		else{
			counter++;
		}
		break;
		
		
		case Decr:
		PORTCout = (PORTCout<= 0) ? PORTCout: PORTCout-1;
		TEMP2 = 10;
		TEMP= 0;
		break;
		
		
		case Reset:
		PORTCout = 0;
		break;
		
		default:
		PORTCout = 0;
		counter =0;
		break;
	}
	state = next_state;
}
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

void TimerISR() {
	TimerFlag = 1;
}

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
	DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output
	
	TimerSet(100);
	TimerOn();
	
	while(1) {
		PORTB = PORTCout;
		PINAtmp = ~PINA & 0x03;
		IncrDecr();
		while(!TimerFlag);
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}
