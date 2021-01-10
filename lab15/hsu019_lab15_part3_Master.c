/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 15 Exercise 3 Master
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
#include "keypad.h"
#include "bit.h"
#endif

static unsigned char pattern = 0x80;	
static unsigned char row = 0xFF;  	
unsigned char mode = 0x00;
unsigned char speed = 0x00;
unsigned char check = 0x00;

unsigned char pattern_array_A[7] = { 0x80,0x40,0x20,0x10,0x04,0x02,0x01 };
unsigned char row_array_A[7] = { 0xE1,0xFA,0xFA,0xE1,0xFF,0xE0,0xFF };

unsigned char pattern_array_B[7] = { 0x80,0x40,0x20,0x10,0x04,0x02,0x01 };
unsigned char row_array_B[7] = { 0xE0,0xEA,0xEA,0xF5,0xFF,0xE0,0xFF };

unsigned char pattern_array_C[7] = { 0x80,0x40,0x20,0x10,0x04,0x02,0x01 };
unsigned char row_array_C[7] = { 0xE0,0xEE,0xEE,0xEE,0xFF,0xE0,0xFF };

unsigned char pattern_array_D[7] = { 0x80,0x40,0x20,0x10,0x04,0x02,0x01 };
unsigned char row_array_D[7] = { 0xE0,0xEE,0xEE,0xF1,0xFF,0xE0,0xFF };

typedef struct task {
	int state; // Current state of the task
	unsigned long period; // Rate at which the task should tick
	unsigned long elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = 0xBF; PORTB = 0x40;

	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	//   of fosc/16  (pg. 168)
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}

void SPI_MasterTransmit(unsigned char cData) {
	// set SS low
	PORTB = SetBit(PORTB, 4, 0);
	// data in SPDR will be transmitted, e.g. SPDR = cData;
	SPDR = cData;
	while (!(SPSR & (1 << SPIF))) { // wait for transmission to complete
		;
	}
	// set SS high
	PORTB = SetBit(PORTB, 4, 1);
}

unsigned char data = 0x00;
enum masterStates { wait, send };
int TickFct_master(int state)
{
	unsigned char x = GetKeypadKey();
	switch (state)
	{
	case wait:
		state = send;
		break;
	case send:
		state = wait;
		break;
	default:
		state = wait;
		break;
	}
	switch (state)
	{
	case wait:
        switch (x)
        {
		case 'A':
			mode = 0x01;
			data = data & 0x0F;
			data = data | 0x10;
			break;
		case 'B':
			mode = 0x02;
			data = data & 0x0F;
			data = data | 0x20;
			break;
		case 'C':
			mode = 0x03;
			data = data & 0x0F;
			data = data | 0x30;
			break;
		case 'D':
			mode = 0x04;
			data = data & 0x0F;
			data = data | 0x40;
			break;
		case '1':
			speed = 0x01;
			data = data & 0xF0;
			data = data | 0x01;
			break;
		case '2':
			speed = 0x02;
			data = data & 0xF0;
			data = data | 0x02;
			break;
		case '3':
			speed = 0x03;
			data = data & 0xF0;
			data = data | 0x03;
			break;
		case '4':
			speed = 0x04;
			data = data & 0xF0;
			data = data | 0x04;
			break;
		case '5':
			speed = 0x05;
			data = data & 0xF0;
			data = data | 0x05;
			break;
		case '6':
			speed = 0x06;
			data = data & 0xF0;
			data = data | 0x06;
			break;
        default:
            break;
        }
		break;
	case send:
		SPI_MasterTransmit(data);
		break;
	default:
		break;
	}
	return state;
}

enum LEDStates_A {init_A,A1,A2,A3,A4,A5,A6,A7} ;
int TickFct_LED_A(int state) {
	switch (state)
	{
	case init_A:
		if (mode == 0x01) {
			state = A1;
		}
		break;
	case A1:
		if (mode == 0x01) {
			state = A2;
		}
		else {
			state = init_A;
		}
		break;
	case A2:
		if (mode == 0x01) {
			state = A3;
		}
		else {
			state = init_A;
		}
		break;
	case A3:
		if (mode == 0x01) {
			state = A4;
		}
		else {
			state = init_A;
		}
		break;
	case A4:
		if (mode == 0x01) {
			state = A5;
		}
		else {
			state = init_A;
		}
		break;
	case A5:
		if (mode == 0x01) {
			state = A6;
		}
		else {
			state = init_A;
		}
		break;
	case A6:
		if (mode == 0x01) {
			state = A7;
		}
		else {
			state = init_A;
		}
		break;
	case A7:
		if (mode == 0x01) {
			state = A1;
		}
		else {
			state = init_A;
		}
		break;
	default:
		state = init_A;
		break;
	}

	switch (state)
	{
	case init_A:
		break;
	case A1:
		pattern = pattern_array_A[0];
		row = row_array_A[0];
		break;
	case A2:
		pattern = pattern_array_A[1];
		row = row_array_A[1];
		break;
	case A3:
		pattern = pattern_array_A[2];
		row = row_array_A[2];
		break;
	case A4:
		pattern = pattern_array_A[3];
		row = row_array_A[3];
		break;
	case A5:
		pattern = pattern_array_A[4];
		row = row_array_A[4];
		break;
	case A6:
		pattern = pattern_array_A[5];
		row = row_array_A[5];
		break;
	case A7:
		pattern = pattern_array_A[6];
		row = row_array_A[6];
		break;
	default:
		break;
	}
	
	PORTC = pattern;
	PORTD = row;
	
	return state;
}

enum LEDStates_B { init_B, B1, B2, B3, B4, B5, B6, B7 };
int TickFct_LED_B(int state) {
	switch (state)
	{
	case init_B:
		if (mode == 0x02) {
			state = B1;
		}
		break;
	case B1:
		if (mode == 0x02) {
			state = B2;
		}
		else {
			state = init_B;
		}
		break;
	case B2:
		if (mode == 0x02) {
			state = B3;
		}
		else {
			state = init_B;
		}
		break;
	case B3:
		if (mode == 0x02) {
			state = B4;
		}
		else {
			state = init_B;
		}
		break;
	case B4:
		if (mode == 0x02) {
			state = B5;
		}
		else {
			state = init_B;
		}
		break;
	case B5:
		if (mode == 0x02) {
			state = B6;
		}
		else {
			state = init_B;
		}
		break;
	case B6:
		if (mode == 0x02) {
			state = B7;
		}
		else {
			state = init_B;
		}
		break;
	case B7:
		if (mode == 0x02) {
			state = B1;
		}
		else {
			state = init_B;
		}
		break;
	default:
		state = init_B;
		break;
	}

	switch (state)
	{
	case init_B:
		break;
	case B1:
		pattern = pattern_array_B[0];
		row = row_array_B[0];
		break;
	case B2:
		pattern = pattern_array_B[1];
		row = row_array_B[1];
		break;
	case B3:
		pattern = pattern_array_B[2];
		row = row_array_B[2];
		break;
	case B4:
		pattern = pattern_array_B[3];
		row = row_array_B[3];
		break;
	case B5:
		pattern = pattern_array_B[4];
		row = row_array_B[4];
		break;
	case B6:
		pattern = pattern_array_B[5];
		row = row_array_B[5];
		break;
	case B7:
		pattern = pattern_array_B[6];
		row = row_array_B[6];
		break;
	default:
		break;
	}

	PORTC = pattern;
	PORTD = row;

	return state;
}

enum LEDStates_C { init_C, C1, C2, C3, C4, C5, C6, C7 };
int TickFct_LED_C(int state) {
	switch (state)
	{
	case init_C:
		if (mode == 0x03) {
			state = C1;
		}
		break;
	case C1:
		if (mode == 0x03) {
			state = C2;
		}
		else {
			state = init_C;
		}
		break;
	case C2:
		if (mode == 0x03) {
			state = C3;
		}
		else {
			state = init_C;
		}
		break;
	case C3:
		if (mode == 0x03) {
			state = C4;
		}
		else {
			state = init_C;
		}
		break;
	case C4:
		if (mode == 0x03) {
			state = C5;
		}
		else {
			state = init_C;
		}
		break;
	case C5:
		if (mode == 0x03) {
			state = C6;
		}
		else {
			state = init_C;
		}
		break;
	case C6:
		if (mode == 0x03) {
			state = C7;
		}
		else {
			state = init_C;
		}
		break;
	case C7:
		if (mode == 0x03) {
			state = C1;
		}
		else {
			state = init_C;
		}
		break;
	default:
		state = init_C;
		break;
	}

	switch (state)
	{
	case init_C:
		break;
	case C1:
		pattern = pattern_array_C[0];
		row = row_array_C[0];
		break;
	case C2:
		pattern = pattern_array_C[1];
		row = row_array_C[1];
		break;
	case C3:
		pattern = pattern_array_C[2];
		row = row_array_C[2];
		break;
	case C4:
		pattern = pattern_array_C[3];
		row = row_array_C[3];
		break;
	case C5:
		pattern = pattern_array_C[4];
		row = row_array_C[4];
		break;
	case C6:
		pattern = pattern_array_C[5];
		row = row_array_C[5];
		break;
	case C7:
		pattern = pattern_array_C[6];
		row = row_array_C[6];
		break;
	default:
		break;
	}

	PORTC = pattern;
	PORTD = row;

	return state;
}

enum LEDStates_D { init_D, D1, D2, D3, D4, D5, D6, D7 };
int TickFct_LED_D(int state) {
	switch (state)
	{
	case init_D:
		if (mode == 0x04) {
			state = D1;
		}
		break;
	case D1:
		if (mode == 0x04) {
			state = D2;
		}
		else {
			state = init_D;
		}
		break;
	case D2:
		if (mode == 0x04) {
			state = D3;
		}
		else {
			state = init_D;
		}
		break;
	case D3:
		if (mode == 0x04) {
			state = D4;
		}
		else {
			state = init_D;
		}
		break;
	case D4:
		if (mode == 0x04) {
			state = D5;
		}
		else {
			state = init_D;
		}
		break;
	case D5:
		if (mode == 0x04) {
			state = D6;
		}
		else {
			state = init_D;
		}
		break;
	case D6:
		if (mode == 0x04) {
			state = D7;
		}
		else {
			state = init_D;
		}
		break;
	case D7:
		if (mode == 0x04) {
			state = D1;
		}
		else {
			state = init_D;
		}
		break;
	default:
		state = init_D;
		break;
	}

	switch (state)
	{
	case init_D:
		break;
	case D1:
		pattern = pattern_array_D[0];
		row = row_array_D[0];
		break;
	case D2:
		pattern = pattern_array_D[1];
		row = row_array_D[1];
		break;
	case D3:
		pattern = pattern_array_D[2];
		row = row_array_D[2];
		break;
	case D4:
		pattern = pattern_array_D[3];
		row = row_array_D[3];
		break;
	case D5:
		pattern = pattern_array_D[4];
		row = row_array_D[4];
		break;
	case D6:
		pattern = pattern_array_D[5];
		row = row_array_D[5];
		break;
	case D7:
		pattern = pattern_array_D[6];
		row = row_array_D[6];
		break;
	default:
		break;
	}

	PORTC = pattern;
	PORTD = row;

	return state;
}


enum speedState { Sp_wait, change };
int TickFct_Speed(int state) {
	switch (state)
	{
	case Sp_wait:
		if (speed == check) {
			state = Sp_wait;
		}
		else
		{
			state = change;
		}
		break;
	case change:
		state = Sp_wait;
		break;
	default:
		state = Sp_wait;
		break;
	}

	switch (state)
	{
	case Sp_wait:
		break;
	case change:
		if (speed == 0x01) {
			row_array_A[4] = 0xFF;
			row_array_A[5] = 0xE0;
			row_array_A[6] = 0xFF;

			row_array_B[4] = 0xFF;
			row_array_B[5] = 0xE0;
			row_array_B[6] = 0xFF;

			row_array_C[4] = 0xFF;
			row_array_C[5] = 0xE0;
			row_array_C[6] = 0xFF;

			row_array_D[4] = 0xFF;
			row_array_D[5] = 0xE0;
			row_array_D[6] = 0xFF;

			check = 0x01;
		}

		if (speed == 0x02) {
			row_array_A[4] = 0xE2;
			row_array_A[5] = 0xEA;
			row_array_A[6] = 0xE8;

			row_array_B[4] = 0xE2;
			row_array_B[5] = 0xEA;
			row_array_B[6] = 0xE8;

			row_array_C[4] = 0xE2;
			row_array_C[5] = 0xEA;
			row_array_C[6] = 0xE8;

			row_array_D[4] = 0xE2;
			row_array_D[5] = 0xEA;
			row_array_D[6] = 0xE8;

			check = 0x02;
		}

		if (speed == 0x03) {
			row_array_A[4] = 0xEA;
			row_array_A[5] = 0xEA;
			row_array_A[6] = 0xE0;

			row_array_B[4] = 0xEA;
			row_array_B[5] = 0xEA;
			row_array_B[6] = 0xE0;

			row_array_C[4] = 0xEA;
			row_array_C[5] = 0xEA;
			row_array_C[6] = 0xE0;

			row_array_D[4] = 0xEA;
			row_array_D[5] = 0xEA;
			row_array_D[6] = 0xE0;

			check = 0x03;
		}

		if (speed == 0x04)
		{
			row_array_A[4] = 0xF8;
			row_array_A[5] = 0xFB;
			row_array_A[6] = 0xE0;

			row_array_B[4] = 0xF8;
			row_array_B[5] = 0xFB;
			row_array_B[6] = 0xE0;

			row_array_C[4] = 0xF8;
			row_array_C[5] = 0xFB;
			row_array_C[6] = 0xE0;

			row_array_D[4] = 0xF8;
			row_array_D[5] = 0xFB;
			row_array_D[6] = 0xE0;

			check = 0x04;
		}

		if (speed == 0x05)
		{
			row_array_A[4] = 0xE8;
			row_array_A[5] = 0xEA;
			row_array_A[6] = 0xE2;

			row_array_B[4] = 0xE8;
			row_array_B[5] = 0xEA;
			row_array_B[6] = 0xE2;

			row_array_C[4] = 0xE8;
			row_array_C[5] = 0xEA;
			row_array_C[6] = 0xE2;

			row_array_D[4] = 0xE8;
			row_array_D[5] = 0xEA;
			row_array_D[6] = 0xE2;

			check = 0x05;
		}

		if (speed == 0x06)
		{
			row_array_A[4] = 0xE0;
			row_array_A[5] = 0xEA;
			row_array_A[6] = 0xE2;

			row_array_B[4] = 0xE0;
			row_array_B[5] = 0xEA;
			row_array_B[6] = 0xE2;

			row_array_C[4] = 0xE0;
			row_array_C[5] = 0xEA;
			row_array_C[6] = 0xE2;

			row_array_D[4] = 0xE0;
			row_array_D[5] = 0xEA;
			row_array_D[6] = 0xE2;

			check = 0x06;
		}

		break;
	default:
		break;
	}

	return state;
}

int main(void) {
	/* Insert DDR and PORT initializations */

	// initialize ports
	DDRA = 0xF0; PORTA = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	SPI_MasterInit();

	static task task1, task2, task3,task4,task5,task6;
	task* tasks[] = { &task1, &task2 ,&task3,&task4,&task5,&task6};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	task1.state = 0;//Task initial state.
	task1.period = 2000;//Task Period.
	task1.elapsedTime = task1.period;//Task current elapsed time.
	task1.TickFct = &TickFct_master;//Function pointer for the tick.

	task2.state = 0;//Task initial state.
	task2.period = 2000;//Task Period.
	task2.elapsedTime = task2.period;//Task current elapsed time.
	task2.TickFct = &TickFct_LED_A;//Function pointer for the tick.

	task3.state = 0;//Task initial state.
	task3.period = 2000;//Task Period.
	task3.elapsedTime = task3.period;//Task current elapsed time.
	task3.TickFct = &TickFct_LED_B;//Function pointer for the tick.

	task4.state = 0;//Task initial state.
	task4.period = 2000;//Task Period.
	task4.elapsedTime = task4.period;//Task current elapsed time.
	task4.TickFct = &TickFct_LED_C;//Function pointer for the tick.

	task5.state = 0;//Task initial state.
	task5.period = 2000;//Task Period.
	task5.elapsedTime = task5.period;//Task current elapsed time.
	task5.TickFct = &TickFct_LED_D;//Function pointer for the tick.

	task6.state = 0;//Task initial state.
	task6.period = 2000;//Task Period.
	task6.elapsedTime = task6.period;//Task current elapsed time.
	task6.TickFct = &TickFct_Speed;//Function pointer for the tick.

	TimerSet(1);
	TimerOn();
	unsigned short i;
	while (1) {
		for (i = 0; i < numTasks; i++) {
			if (tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 2000;
		}
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
