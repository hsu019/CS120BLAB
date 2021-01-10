/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #5
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
	DDRD = 0x00; PORTD = 0xFF; 
	DDRB = 0xFE;PORTB = 0x01;
	unsigned int tmpD = 0x00; 
	unsigned int tmpB = 0x00;
while(1) {
		tmpD = PIND << 1;
		tmpB = tmpD + (PINB & 0x01);

		if(tmpB >=70){
            PORTB = 0x02;
        }
        else if(tmpB>5 && tmpB<70){
            PORTB = 0x04;
        }
        else{
            PORTB = 0;
        }
	}
    return 1;
}
