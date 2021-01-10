/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1DFZOOeRncg4JW9zlDIliKS9P5avbRLwA/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char pattern_array[4] = { 0x20,0x10,0x08,0x04 };
unsigned char row_array[4] = { 0xF1,0xF5,0xF5,0xF1 };


enum DemoStates { init, s1, s2, s3, s4 } LEDstate;
void Demo_Tick()
{
	static unsigned char pattern = 0x80;
	static unsigned char row = 0x00;
	switch (LEDstate)
	{
	case init:
		LEDstate = s1;
		break;
	case s1:
		LEDstate = s2;
		break;
	case s2:
		LEDstate = s3;
		break;
	case s3:
		LEDstate = s4;
		break;
	case s4:
		LEDstate = s1;
		break;
	default:
		LEDstate = init;
		break;

	}
	switch (LEDstate)
	{
	case init:
		break;
	case s1:
		pattern = pattern_array[0];
		row = row_array[0];
		break;
	case s2:
		pattern = pattern_array[1];
		row = row_array[1];
		break;
	case s3:
		pattern = pattern_array[2];
		row = row_array[2];
		break;
	case s4:
		pattern = pattern_array[3];
		row = row_array[3];
		break;
	default:
		break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
}





int main(void) {
	/* Insert DDR and PORT initializations */
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	//DDRA = 0x00;
	//PORTA = 0xFF;
	TimerSet(1);
	TimerOn();
	while (1) {
		//tmpA = ~PINA;
		Demo_Tick();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
