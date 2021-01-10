/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 9 Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1QUlIyXBzGbqWZEUJ0DTYHR59_TRWo9cU/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task {
	int state; // Current state of the task
	unsigned long period; // Rate at which the task should tick
	unsigned long elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

task tasks[3];

const unsigned char tasksNum = 3;
const unsigned long tasksPeriodGCD = 100;
const unsigned long periodBlinkLED = 1000;
const unsigned long periodThreeLEDs = 300;
const unsigned long periodCB = 100;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;

	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	unsigned char i;
	for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
		if (tasks[i].elapsedTime >= tasks[i].period) { // Ready
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasksPeriodGCD;
	}
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char BL_tmpB;
enum BL_States { BL_Start, BL_Init, On, Off };

int TickFct_BlinkLed(int state) {
	switch (state) {
		case BL_Start:
			state = BL_Init;
			break;
		case BL_Init:
			state = On;
			break;
		case On:
			state = Off;
			break;
		case Off:
			state = On;
			break;
		default:
			state = BL_Start;
			break;
	}

	switch (state) {
		case BL_Init:
			break;
		case On:
			BL_tmpB = 0x08;
			break;
		case Off:
			BL_tmpB = 0x00;
			break;
		default:
			break;
	}

	return state;

}

unsigned char TL_tmpB;
enum TL_States { TL_Start, TL_Init, One, Two, Three };

int TickFct_ThreeLeds(int state) {
	switch (state) {
		case TL_Start:
			state = TL_Init;
			break;
		case TL_Init:
			state = One;
			break;
		case One:
			state = Two;
			break;
		case Two:
			state = Three;
			break;
		case Three:
			state = One;
			break;
		default:
			state = TL_Start;
			break;
	}

	switch (state) {
		case TL_Init:
			break;
		case One:
			TL_tmpB = 0x01;
			break;
		case Two:
			TL_tmpB = 0x02;
			break;
		case Three:
			TL_tmpB = 0x04;
			break;
		default:
			break;
	}

	return state;

}



unsigned char tmpB;
enum CB_States { CB_Start, CB } CB_State;

void TickFct_Combine_State() {
	switch (CB_State) {
	case CB_Start:
		CB_State = CB;
		break;
	case CB:
		CB_State = CB;
		break;
	default:
		break;
	}
	switch (CB_State) {
	case CB_Start:
		PORTB = 0x00;
		break;
	case CB:
		tmpB = TL_tmpB | BL_tmpB;
		PORTB = tmpB;
		break;
	default:
		break;
	}
}

int main(void) {
	/* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;

	unsigned char i = 0;
	tasks[i].state = BL_Start;
	tasks[i].period = periodBlinkLED;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_BlinkLed;
	++i;
	tasks[i].state = TL_Start;
	tasks[i].period = periodThreeLEDs;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_ThreeLeds;
	++i;
	tasks[i].state = CB_Start;
	tasks[i].period = periodCB;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Combine_State;


	TimerSet(tasksPeriodGCD);
	TimerOn();
	while (1) {
		TickFct_Combine_State();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
