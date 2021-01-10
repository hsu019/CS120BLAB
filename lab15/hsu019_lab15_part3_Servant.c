/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 15 Exercise 3 Servant
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/113AWYEQGWYrDuXpvr1HZC8vvQyrWwYUz/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#endif

unsigned char receivedData;
unsigned char temp;
unsigned char speed;
unsigned char pattern;
unsigned char data = 0xF0;
unsigned char start = 0x00;
unsigned char order = 0x01;
unsigned int speeder = 2000;

typedef struct task {
	int state; // Current state of the task
	unsigned long period; // Rate at which the task should tick
	unsigned long elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1 << SPE) | (1 << SPIE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}
ISR(SPI_STC_vect) { // this is enabled in with the SPCR register?s ?SPI
	// Interrupt Enable?
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	receivedData = SPDR;
}

enum servantStates { s_wait, read } ;
int TickFct_servant(int state)
{
	switch (state)
	{
	case s_wait:
		state = read;
		break;
	case read:
		state = s_wait;
		break;
	default:
		state = s_wait;
		break;
	}
	switch (state)
	{
	case s_wait:
		break;
	case read:
		temp = receivedData;
		speed = temp & 0x0F;
		pattern = (temp >> 4) & 0x0F;
		break;
	default:
		break;
	}
	return state;
}

enum patternStates { pattern0, pattern1,pattern2,pattern3,pattern4 };
int TickFct_pattern(int state)
{
	switch (state)
	{
	case pattern0:
		if (pattern == 0x01)
		{
			state = pattern1;
		}
		if (pattern == 0x02)
		{
			state = pattern2;
		}
		if (pattern == 0x03)
		{
			state = pattern3;
		}
		if (pattern == 0x04)
		{
			state = pattern4;
		}
		break;
	case pattern1:
		if (pattern == 0x01)
		{
			state = pattern1;
		}
		if (pattern == 0x02)
		{
			state = pattern2;
		}
		if (pattern == 0x03)
		{
			state = pattern3;
		}
		if (pattern == 0x04)
		{
			state = pattern4;
		}
		break;
	case pattern2:
		if (pattern == 0x01)
		{
			state = pattern1;
		}
		if (pattern == 0x02)
		{
			state = pattern2;
		}
		if (pattern == 0x03)
		{
			state = pattern3;
		}
		if (pattern == 0x04)
		{
			state = pattern4;
		}
		break;
	case pattern3:
		if (pattern == 0x01)
		{
			state = pattern1;
		}
		if (pattern == 0x02)
		{
			state = pattern2;
		}
		if (pattern == 0x03)
		{
			state = pattern3;
		}
		if (pattern == 0x04)
		{
			state = pattern4;
		}
		break;
	case pattern4:
		if (pattern == 0x01)
		{
			state = pattern1;
		}
		if (pattern == 0x02)
		{
			state = pattern2;
		}
		if (pattern == 0x03)
		{
			state = pattern3;
		}
		if (pattern == 0x04)
		{
			state = pattern4;
		}
		break;
	default:
		state = pattern0;
		break;
	}
	switch (state)
	{
	case pattern0:
		start = 0x00;
		order = 0x01;
		PORTA = 0x00;
		break;
	case pattern1:
		start = 0x00;
		order = 0x01;
		if (data == 0xF0) {
			data = 0x0F;
		}
		else
		{
			data = 0xF0;
		}
		PORTA = data;
		break;
	case pattern2:
		start = 0x00;
		order = 0x01;
		if (data == 0xAA) {
			data = 0x55;
		}
		else {
			data = 0xAA;
		}
		PORTA = data;
		break;
	case pattern3:
		if (start == 0x00) {
			data = 0x80;
			start = 0x01;
			PORTA = data;
		}
		else {
			if (order == 0x01) {
				if (data != 0x01) {
					data = (data >> 1);
					PORTA = data;
				}
				else
				{
					PORTA = data;
					order = 0x00;
				}
			}
			else {
				if (data != 0x80) {
					data = (data << 1);
					PORTA = data;
				}
				else
				{
					PORTA = data;
					order = 0x01;
				}
			}
		}
		break;
	case pattern4:
		start = 0x00;
		order = 0x01;
		if (data == 0xFF) {
			data = 0x00;
		}
		else {
			data = 0xFF;
		}
		PORTA = data;
		break;
	default:
		break;
	}
	return state;
}

enum speedStates {smode1,smode2,smode3,smode4,smode5,smode6};
int TickFct_speed(int state) {
	switch (state)
	{
	case smode1:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	case smode2:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	case smode3:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	case smode4:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	case smode5:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	case smode6:
		if (speed == 0x01) {
			state = smode1;
		}
		if (speed == 0x02) {
			state = smode2;
		}
		if (speed == 0x03) {
			state = smode3;
		}
		if (speed == 0x04) {
			state = smode4;
		}
		if (speed == 0x05) {
			state = smode5;
		}
		if (speed == 0x06) {
			state = smode6;
		}
		break;
	default:
		state = smode1;
		break;
	}

	switch (state)
	{
	case smode1:
		speeder = 2000;
		break;
	case smode2:
		speeder = 1000;
		break;
	case smode3:
		speeder = 500;
		break;
	case smode4:
		speeder = 250;
		break;
	case smode5:
		speeder = 100;
		break;
	case smode6:
		speeder = 50;
		break;
	default:
		break;
	}

	return state;
}


int main(void) {
	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	SPI_ServantInit();

	static task task1, task2, task3;
	task* tasks[] = { &task1, &task2, &task3 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = 0;//Task initial state.
	task1.period = 1000;//Task Period.
	task1.elapsedTime = task1.period;//Task current elapsed time.
	task1.TickFct = &TickFct_servant;//Function pointer for the tick.

	task2.state = 0;//Task initial state.
	task2.period = speeder;//Task Period.
	task2.elapsedTime = task2.period;//Task current elapsed time.
	task2.TickFct = &TickFct_pattern;//Function pointer for the tick.

	task3.state = 0;//Task initial state.
	task3.period = 50;//Task Period.
	task3.elapsedTime = task3.period;//Task current elapsed time.
	task3.TickFct = &TickFct_speed;//Function pointer for the tick.

	TimerSet(50);
	TimerOn();
	unsigned short i;
	while (1) {
		task2.period = speeder;
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 50;
		}
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
