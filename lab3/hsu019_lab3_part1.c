/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;
	unsigned char cnt;
	while(1) {
		tmpA = PINA;
		tmpB = PINB;
		cnt = 0x00;
		while (tmpA > 0)
		{
			if((tmpA & 0x01) == 1 ) {
				cnt++;
			}
			tmpA = tmpA >> 1;
		}
		//PORTC = cnt;
		while (tmpB > 0)
		{	
			if((tmpB & 0x01) == 1){
				cnt++;
			}
			tmpB = tmpB >> 1;
		}

		PORTC = cnt;	
	}
    return 1;
}
