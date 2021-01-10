/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1HmegZ7gng8g1fo1U8yh2QFfJ3nXheWSD/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

typedef struct task {
	int state; // Current state of the task
	unsigned long period; // Rate at which the task should tick
	unsigned long elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

unsigned short joystick = 0x0000;
unsigned char pattern = 0x01;
unsigned char row = 0x10;
unsigned short temp = 0;
unsigned int speeder = 1000;

void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for (i = 0; i < 15; i++) { asm("nop"); }
}

void convert()
{
	ADCSRA |= (1 << ADSC);//start ADC conversion
	while (!(ADCSRA & (1 << ADIF)));//wait till ADC conversion

}


enum LRStates { leftRight, upDown ,both};
int LR_Tick(int state)
{
	switch (state)
	{
	case leftRight:
		Set_A2D_Pin(0x00);
		convert();
		joystick = ADC;
		if (((joystick > 700) && (joystick < 750)) || ((joystick < 450) && (joystick > 350)))
			speeder = 1000;
		else if (((joystick > 750) && (joystick < 850)) || ((joystick < 350) && (joystick > 250)))
			speeder = 500;
		else if (((joystick > 850) && (joystick < 925)) || ((joystick < 250) && (joystick > 175)))
			speeder = 250;
		else if ((joystick > 925) || (joystick < 175))
			speeder = 100;
		if (joystick < 350)
		{
			if (row != 0x10)
				row = row << 1;
			if (pattern != 0x80)
				pattern = pattern << 1;

		}
		else if (joystick > 700)
		{
			if (row != 0x01)
				row = row >> 1;
			if (pattern != 0x01)
				pattern = pattern >> 1;
		}
		state = upDown;
		break;
	case upDown:
		Set_A2D_Pin(0x01);
		convert();
		temp = ADC;
		if (((temp > 700) && (temp < 750)) || ((temp < 450) && (temp > 350)))
			speeder = 1000;
		else if (((temp > 750) && (temp < 850)) || ((temp < 350) && (temp > 250)))
			speeder = 500;
		else if (((temp > 850) && (temp < 925)) || ((temp < 250) && (temp > 175)))
			speeder = 250;
		else if ((temp > 925) || (temp < 175))
			speeder = 100;
		if (temp < 350)
		{
			if (row != 0x10)
				row = row << 1;
			if (pattern != 0x80)
				pattern = pattern << 1;
		}
		else if (temp > 700)
		{
			if (row != 0x01)
				row = row >> 1;
			if (pattern != 0x01)
				pattern = pattern >> 1;
		}
		state = leftRight;
		break;
	case both:
		Set_A2D_Pin(0x03);
		convert();
		temp = ADC;
		if (((temp > 700) && (temp < 750)) || ((temp < 450) && (temp > 350)))
			speeder = 1000;
		else if (((temp > 750) && (temp < 850)) || ((temp < 350) && (temp > 250)))
			speeder = 500;
		else if (((temp > 850) && (temp < 925)) || ((temp < 250) && (temp > 175)))
			speeder = 250;
		else if ((temp > 925) || (temp < 175))
			speeder = 100;
		if (temp < 350)
		{
			if (row != 0x10)
				row = row << 1;
			if (pattern != 0x80)
				pattern = pattern << 1;
		}
		else if (temp > 700)
		{
			if (row != 0x01)
				row = row >> 1;
			if (pattern != 0x01)
				pattern = pattern >> 1;
		}
		state = both;
		break;
	default:
		break;
	}
	return state;
}

enum LEDMatrixStates { display };
int LEDMatrix_Tick(int state)
{
	switch (state)
	{
	case display:
		break;
	default:
		break;
	}
	switch (state)
	{
	case display:
		PORTC = pattern;
		PORTD = ~row;
		break;
	default:
		break;
	}
	return state;
}



int main(void) {
    /* Insert DDR and PORT initializations */
	A2D_init();
	DDRA = 0x00;PORTA = 0xFF;
	DDRC = 0xFF;PORTC = 0x00;
    DDRD = 0xFF;PORTD = 0x00;

	static task task1, task2;
	task* tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = 0;//Task initial state.
	task1.period = speeder;//Task Period.
	task1.elapsedTime = task1.period;//Task current elapsed time.
	task1.TickFct = &LR_Tick;//Function pointer for the tick.

	task2.state = 0;//Task initial state.
	task2.period = 1;//Task Period.
	task2.elapsedTime = task2.period;//Task current elapsed time.
	task2.TickFct = &LEDMatrix_Tick;//Function pointer for the tick.

    TimerSet(1);
    TimerOn();
	unsigned short i;
    while (1) {
		task1.period = speeder;
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
