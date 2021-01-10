/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 7  Exercise 4
 *
 * Exercise Description: [optional - include for your own benefit]
 * In my case, ADC(MAX) = 10 0001 1111, which is 527.
 * ADC(MIN) = 00 0011 1111, which is 63
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/10qtlPwPRaxovvvg6Ios0AGYTTo6xoDIB/view?usp=sharing
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
		if (x < 66)
		{
			tmpB = 0x01;
			PORTB = tmpB;
		}
		else if (x >= 66 && x < 132)
		{
			tmpB = 0x03;
			PORTB = tmpB;
		}
		else if (x >= 132 && x < 198)
		{
			tmpB = 0x07;
			PORTB = tmpB;
		}
		else if (x >= 198 && x < 264)
		{
			tmpB = 0x0F;
			PORTB = tmpB;
		}
		else if (x >= 264 && x < 330)
		{
			tmpB = 0x1F;
			PORTB = tmpB;
		}
		else if (x >= 330 && x < 396)
		{
			tmpB = 0x3F;
			PORTB = tmpB;
		}
		else if (x >= 396 && x < 462)
		{
			tmpB = 0x7F;
			PORTB = tmpB;
		}
		else {
			tmpB = 0xFF;
			PORTB = tmpB;
		}
    }
    return 0;
}
