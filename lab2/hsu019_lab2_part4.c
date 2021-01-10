/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #2  Exercise #4
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
    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0x00;PORTB = 0xFF;
    DDRC = 0x00;PORTC = 0xFF;
    DDRD = 0xFF;PORTD = 0x00;

    unsigned char weight =0x00;
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
    while (1) {
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
        weight = tmpA + tmpB + tmpC;
        if(((tmpA - tmpC) > 80) || ((tmpC - tmpA)>80)){
            tmpD = 0x02;
        }
        else{
            tmpD = 0x00;
        }
        if(weight> 140){
            tmpD = tmpD + 0x01;
        }
        else{
            tmpD = tmpD;
        }
        tmpD = tmpD + ((weight >> 2) & 0xFC);
		PORTD = tmpD;
       
    }
    return 1;
}