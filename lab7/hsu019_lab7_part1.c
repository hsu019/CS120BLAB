/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 7  Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1ex4cYjWJ2WqkBPvzKruOu2o8h8x_Dihe/view?usp=sharing
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
	DDRD = 0xFF;
	PORTD = 0x00;
	ADC_init();
	unsigned char tmpB;
	unsigned char tmpD;
	unsigned short x;
    while(1){
		x = ADC;
		tmpB = (char)x;
		tmpD = (char)(x >> 8);
		PORTB = tmpB;
		PORTD = tmpD;
    }
    return 0;
}
