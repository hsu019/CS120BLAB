/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 10 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1uuFMWhXx5q87Vl4GH66s30yjyHxblCFs/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

typedef struct task {
	signed char state; // Current state of the task
	unsigned long int period; // Rate at which the task should tick
	unsigned long int elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum pauseButtonSM_States {pauseButton_wait, pauseButtion_press, pauseButtion_release};

int pauseButtonSMTick(int state) {
	unsigned char press = ~PINA & 0x01;

	switch (state)
	{
	case pauseButton_wait:
		state = press == 0x01 ? pauseButtion_press : pauseButton_wait;
		break;
	
	case pauseButtion_press:
		state = pauseButtion_release;
		break;

	case pauseButtion_release:
		state = press == 0x00 ? pauseButton_wait : pauseButtion_press;
		break;

	default:
		state = pauseButton_wait;
		break;
	}

	switch (state)
	{
	case pauseButton_wait:
		break;
	case pauseButtion_press:
		pause = (pause == 0) ? 1 : 0;
		break;
	case pauseButtion_release:
		break;
	default:
		break;
	}
	return state;
}

enum toggleLED0_States { toggleLED0_wait, toggleLED0_blink};

int toggleLED0SMTick(int state) {
	switch (state)
	{
	case toggleLED0_wait: state = !pause ? toggleLED0_blink : toggleLED0_wait;
		break;
	case toggleLED0_blink:state = pause ? toggleLED0_wait : toggleLED0_blink;
		break;
	default:
		state = toggleLED0_wait;
		break;
	}
	switch (state)
	{
	case toggleLED0_wait:
		break;
	case toggleLED0_blink:
		led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
		break;
	default:
		break;
	}

	return state;

}

enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink};

int toggleLED1SMTick(int state) {
	switch (state)
	{
	case toggleLED1_wait: state = !pause ? toggleLED1_blink : toggleLED1_wait;
		break;
	case toggleLED1_blink: state = pause ? toggleLED1_wait : toggleLED1_blink;
		break;
	default:
		state = toggleLED1_wait;
		break;
	}

	switch (state)
	{
	case toggleLED1_wait:
		break;
	case toggleLED1_blink:
		led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
		break;
	default:
		break;
	}

	return state;

}

enum display_States {display_display};

int displaySMTick(int state) {
	unsigned char output;

	switch (state)
	{
	case display_display: state = display_display;
		break;
	default:
		state = display_display;
		break;
	}

	switch (state)
	{
	case display_display:
		output = led0_output | led1_output << 1;
		break;
	default:
		break;
	}
	PORTB = output;
	return state;
}


int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonSMTick;

	task2.state = start;
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.TickFct = &toggleLED0SMTick;

	task3.state = start;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.TickFct = &toggleLED1SMTick;

	task4.state = start;
	task4.period = 10;
	task4.elapsedTime = task4.period;
	task4.TickFct = &displaySMTick;



	TimerSet(10);
	TimerOn();

	unsigned short i;
	while (1) {
		for (i = 0; i < numTasks; i++)
		{
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 10;
		}
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}