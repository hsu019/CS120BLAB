/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #2  Exercise #2
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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00;
	unsigned char cntavail;
    /* Insert your solution below */
    while (1) {
		tmpA = PINA;
		cntavail = 0x04;
		if ((tmpA & 0x01) == 0x01) { 
			cntavail--;
		}
		if ((tmpA & 0x02) == 0x02) { 
			cntavail--;
		}
		if ((tmpA & 0x04) == 0x04) { 
			cntavail--;
		}
		if ((tmpA & 0x08) == 0x08) { 
			cntavail--;
		}	
		PORTC = cntavail;
    }
    return 1;
}
