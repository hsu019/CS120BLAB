/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 7  Exercise 3
 *
 * Exercise Description: [optional - include for your own benefit]
 * In my case, ADC(MAX) = 10 0001 1111, which is 527.
 * ADC(MIN) = 00 0011 1111, which is 63
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/13nIem4sljzP5BQjx_jdKZBkqSrb-0mbc/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif




void ADC_init(){
    ADCSRA |= (1 << ADEN ) | (1 << ADSC) | (1 << ADATE);
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
	ADC_init();
	unsigned char tmpB;
	unsigned short x;
    while(1){
		x = ADC;
		if (x >= (527 / 2))
		{
			tmpB = 0x01;
			PORTB = tmpB;
		}
		else {
			tmpB = 0x00;
			PORTB = tmpB;
		}
    }
    return 0;
}
