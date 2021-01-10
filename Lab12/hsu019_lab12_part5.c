/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 12 Exercise 5
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1erCgUJbIHgs1A_7N2ZviSBiNtC3E6Nnb/view?usp=sharing
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

unsigned char pattern_array[4] = { 0x20,0x10,0x08,0x04 };
unsigned char row_array[4] = { 0x8F,0xAF,0xAF,0x8F };
unsigned char tmpA;

void transmit_data(unsigned char data) {
	int i;
	for (i = 0; i < 8; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from ¡°Shift¡± register to ¡°Storage¡± register
	PORTC |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

void transmit_data1(unsigned char data) {
	int i;
	for (i = 0; i < 8; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x08;
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from ¡°Shift¡± register to ¡°Storage¡± register
	PORTD |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}


enum Operation { shift, up, down, left, right, wait };
int Operation_Tick(int state) {
	int i;
	switch (state)
	{
	case shift:
		if (tmpA == 0x01)
		{
			state = up;
		}
		else if (tmpA == 0x02)
		{
			state = down;
		}
		else if (tmpA == 0x04) {
			state = left;
		}
		else if (tmpA == 0x08)
		{
			state = right;
		}
		else
		{
			state = shift;
		}
		break;
	case up:
		state = wait;
		break;
	case down:
		state = wait;
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

	switch (state) {
	case shift:
		break;
	case up:
		if (row_array[0] != 0x1F)
		{
			
			for (i = 0; i < 4; i++) {
				row_array[i] = (row_array[i] << 1) | 0x01;
			}
		}
		break;
	case down:
		if (row_array[0] != 0xC7)
		{
			for (i = 0; i < 4; i++)
			{
				row_array[i] = (row_array[i] >> 1) | 0x80;
			}
		}
		break;
	case left:
		if ((pattern_array[3] & 0x01) != 0x01)
		{
			for (i = 0; i < 4; i++) {
				pattern_array[i] = (pattern_array[i] >> 1);
			}
		}
		break;
	case right:
		if ((pattern_array[0] & 0x80) != 0x80)
		{
			for (i = 0; i < 4; i++) {
				pattern_array[i] = (pattern_array[i] << 1);
			}
		}
		
		break;
	case wait:
		break;
	default:
		break;
	}

	return state;
}

enum DemoStates { init, s1, s2, s3, s4 };
int Demo_Tick(int state)
{
	static unsigned char pattern = 0x80;
	static unsigned char row = 0x00;
	switch (state)
	{
	case init:
		state = s1;
		break;
	case s1:
		state = s2;
		break;
	case s2:
		state = s3;
		break;
	case s3:
		state = s4;
		break;
	case s4:
		state = s1;
		break;
	default:
		state = init;
		break;

	}
	switch (state)
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
	transmit_data(pattern);	// Pattern to display
	transmit_data1(row);		// Row(s) displaying pattern	

	return state;
}






int main(void) {
	/* Insert DDR and PORT initializations */
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;

	static task task1, task2;
	task* tasks[] = { &task1, &task2 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = 0;//Task initial state.
	task1.period = 1;//Task Period.
	task1.elapsedTime = task1.period;//Task current elapsed time.
	task1.TickFct = &Demo_Tick;//Function pointer for the tick.

	task2.state = 0;//Task initial state.
	task2.period = 1;//Task Period.
	task2.elapsedTime = task2.period;//Task current elapsed time.
	task2.TickFct = &Operation_Tick;//Function pointer for the tick.

	TimerSet(1);
	TimerOn();
	unsigned short i;
	while (1) {
		tmpA = ~PINA;
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
