/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #3
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
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	unsigned char tmpA = 0x00; 
	unsigned char upper;
	unsigned char lower;
while(1) {
		tmpA = PINA;
		upper = 0x00;
		lower = 0x00;

		lower = (tmpA & 0xF0) >> 4;
		upper = (tmpA & 0x0F) << 4;

		PORTB = lower;
		PORTC = upper;	
	}
    return 1;
}
