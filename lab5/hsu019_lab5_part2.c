/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  demo video link: https://drive.google.com/file/d/1ji3EVaHukDfPRMXOHrEkhtINl1LOYE9j/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


enum States { Start, Init, Wait, Increase, Decrease, Reset } State;
unsigned char count = 0x07;
unsigned char stay;
unsigned char tmpA = 0x00;


void Tick()
{
	 switch(State) {
		 case Start:
			 State = Wait;
			 break;
		 case Increase:
			 State = Wait;
		 	 stay = 1;
			 break;
		 case Decrease:
			 State = Wait;
			 stay = 1;
			 break;
		 case Reset:
			 State = Wait;
		     stay = 1;
			 break;
		 case Wait:
			 if (tmpA == 0x01 && !stay) {
			 	State = Increase;
			 }
			 else if (tmpA == 0x02 && !stay) {
				State = Decrease;
			 }
			 else if (tmpA == 0x03)
			 {
				 State = Reset;
			 }
			 else if (tmpA == 0x00)
			 {
				 stay = 0;
				 State = Wait;
			 }
			 else {
				State = Wait;
			 }
			 break;
		 default:
			 State = Start;
			 break;
	}

	 switch(State) {
	 case Increase:
		 if (count < 0x09)
		 {
		 	count++;
	 		PORTC = count;
		 }
		 else {
			PORTC = count;
		 }
	 	 break;
	 case Decrease:
		 if (count > 0x00)
		 {
		 	count = count - 1;
	 		PORTC = count;
		 }
		 else {
			PORTC = count;
		 }
	     break;
	 case Reset:
		 count = 0x00;
	 	 PORTC = count;
	     break;
	 case Wait:
	 	 PORTC = count;
	     break;
	 default:
		 break;
	 }
	}



int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRC = 0xFF;PORTC = 0x00;
	State = Start; 
    while(1){
		tmpA = ~PINA;
        Tick();
    }
    return 0;
}
