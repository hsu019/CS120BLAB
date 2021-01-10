/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States { Start, OFF_RELEASE, ON_PRESS, ON_RELEASE, OFF_PRESS } State;
void Tick()
{
	 switch(State) {
		 case Start:
			 State = OFF_RELEASE;
			 PORTB = 0x01;
			 break;
		 case OFF_RELEASE:
			 if (PINA == 0x01) {
				State = ON_PRESS;
				PORTB = 0x02;
			 }
			 else  {
				State = OFF_RELEASE;
				PORTB = 0x01;
			 }
			 break;
		 case ON_PRESS:
			 if (PINA == 0x01) {
			 	State = ON_PRESS;
				PORTB = 0x02;
			 }
			 else {
				State = ON_RELEASE;
				PORTB = 0x02;
			 }
			 break;
		 case ON_RELEASE:
			 if (PINA == 0x01) {
				State = OFF_PRESS;
				PORTB = 0x01;
			 }
			 else {
				State = ON_RELEASE;
				PORTB = 0x02;
			 }
			 break;
		 case OFF_PRESS:
			 if (PINA == 0x00) {
				State = OFF_RELEASE;
				PORTB = 0x01;
			 }
			 else {
				State = OFF_PRESS;
				PORTB = 0x01;
			 }
			 break;
		 default:
			 State = Start;
			 break;
	}

	 switch(State) {
	 case OFF_RELEASE:
		 break;
	 case ON_PRESS:
	     break;
	 case ON_RELEASE:
		 break;
	 case OFF_PRESS:
		 break;
	 default:
		 break;
	 }
	}



int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0xFF;PORTB = 0x00;
	State = Start; 
    while(1){
        Tick();
    }
    return 0;
}

