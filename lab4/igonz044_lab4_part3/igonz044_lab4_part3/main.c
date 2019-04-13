 /**
 * Created: 4/19/2018 7:48:48 AM
 * Author : Tanish
 */ 

#include <avr/io.h>

enum LA_States { LA_SMStart, init, check, increment, decrement, wait, wait1, reset } LA_State;

#define A0 (~PINA & 0x01)
#define A1 (~PINA & 0x02)

void TickFct_LED()
{
	switch(LA_State) {   // Transitions
		case LA_SMStart:  // Initial transition
		LA_State = init;
		break;

		case init:
		LA_State = check;
		break;
		
		case check:
		if(!A0 && !A1){
			LA_State = check;
			}else if(A0 && !A1){
			LA_State = increment;
			}else if(!A0 && A1){
			LA_State = decrement;
			}else if(A0 && A1){
			LA_State = reset;
		}
		break;
		
		case increment:
		LA_State = wait;
		break;
		
		case decrement:
		LA_State = wait1;
		break;
		
		case wait:
		if(A0 && A1){
			LA_State = reset;
			}else if(!A0 ){ //&& !A1
			LA_State = check;
			}else if (A0 ){//&& !A1
			LA_State = wait;
		}
		break;
		
		case wait1:
		if(A0 && A1){
			LA_State = reset;
			}else if(!A1){
			LA_State = check;
			}else if(A1){
			LA_State = wait1;
		}
		break;
		
		case reset:
		LA_State = check;
		break;

		default:
		LA_State = LA_SMStart;
		break;
	} // Transitions

	switch(LA_State) {   // State actions
		case init:
		PORTB = 0x07;
		break;

		case check:
		break;
		
		case increment:
		if( PORTB < 9 ){
			PORTB = PORTB + 1;
		}
		break;
		
		case decrement:
		if( PORTB > 0 ){
			PORTB = PORTB - 1;
		}
		break;
		
		case wait:
		break;
		
		case wait1:
		break;
		
		case reset:
		PORTB = 0x00;
		break;

		default:
		break;
	} // State actions
}

int main(void) {
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	PORTB = 0x07;              // Initialize outputs
	
	LA_State = LA_SMStart; // Indicates initial call

	while(1) {
		TickFct_LED();
	}
}