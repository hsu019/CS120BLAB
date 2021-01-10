/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/16RD_UwV2dAF8BR7bAfawXsPk58GHv24G/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char tmpA;
enum Demo_States { shift,left, right,wait }state;
void Demo_Tick() {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xE0;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
	case shift:
		if (tmpA == 0x04)
		{
			state = left;
		}
		else if (tmpA == 0x08)
		{
			state = right;
		}
		else {
			state = shift;
		}
		break;
	case left:
		state = wait;
		break;
	case right:
		state = wait;
		break;
	case wait:
		if (tmpA == 0x00)
		{
			state = shift;
		}
		break;
	default:
		state = shift;
		break;
	}
	// Actions
	switch (state) {
	case shift:
		PORTC = pattern;	
		PORTD = row;
		break;
	case left:
		if (pattern < 0x80)
		{
			pattern = (pattern << 1);
		}
		break;
	case right:
		if (pattern > 0x01)
		{
			pattern = (pattern >> 1);
		}
		break;
	case wait:
		break;
	default:
		break;
	}


}





int main(void) {
    /* Insert DDR and PORT initializations */
    DDRD = 0xFF;
    PORTD = 0x00;
    DDRC = 0xFF;
    PORTC = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;
    TimerSet(100);
    TimerOn();
    while (1) {
		tmpA = ~PINA;
		Demo_Tick();
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
