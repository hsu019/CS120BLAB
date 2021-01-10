/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1Wey4JQ7UsYgGY5-mvauFEqVOwn5fNPEH/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned short temp = 0;

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

void convert()
{
	ADCSRA |= (1 << ADSC);//start ADC conversion
	while (!(ADCSRA & (1 << ADIF)));//wait till ADC conversion

}

enum DisplayStates { init } state;
void Display_Tick()
{
	switch (state)
	{
	case init:
		state = init;
		break;
	default:
		break;
	}
	switch (state)
	{
	case init:
		//convert();
		temp = ADC;
		PORTB = temp;
		PORTD = temp >> 8;
		break;
	default:
		break;
	}
}





int main(void) {
    /* Insert DDR and PORT initializations */
	A2D_init();
	DDRA = 0xFF;PORTA = 0x00;
	DDRB = 0xFF;PORTB = 0x00;
    DDRD = 0xFF;PORTD = 0x00;
    TimerSet(100);
    TimerOn();
    while (1) {
		Display_Tick();
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
