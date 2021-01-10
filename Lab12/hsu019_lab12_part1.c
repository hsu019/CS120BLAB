/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1zQqYTSBB5tJdGdhVwf0S7aJvI4aRqSLd/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char tmpA;
enum Demo_States { shift,up, down,wait }state;
void Demo_Tick() {

	// Local Variables
	static unsigned char pattern = 0xFF;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
	case shift:
		if (tmpA == 0x01)
		{
			state = up;
		}
		else if (tmpA == 0x02)
		{
			state = down;
		}
		else {
			state = shift;
		}
		break;
	case up:
		state = wait;
		break;
	case down:
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
	case up:
		if (row < 0xFE)
		{
			row = (row >> 1) | 0x80;
		}
		break;
	case down:
		if (row > 0xEF)
		{
			row = (row << 1) | 0x01;
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
