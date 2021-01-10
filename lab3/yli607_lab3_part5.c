/*	Author: Yunxuan Li
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 3  Exercise 1
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
    DDRD = 0x00;PORTD = 0xFF;
    DDRB = 0xFE;PORTB = 0x01;
    unsigned int weight;
    unsigned int leading_bit;
    while (1) {
        leading_bit = PIND << 1;
        weight = leading_bit + (PINB &0x01);
        if(weight >=70){
            PORTB = 0x02;
        }
        else if(weight>5 && weight<70){
            PORTB = 0x04;
        }
        else{
            PORTB = 0;
        }
    }
    return 0;
}
