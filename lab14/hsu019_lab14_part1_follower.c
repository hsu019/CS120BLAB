/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 14 Exercise 1 follower
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1OTUIaw6Wroq76qJ_Nh30X6VMTtJHEVwC/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "usart_ATmega1284.h"
#endif

unsigned char temp;
enum Follower_States { on } state;

void Follower_Tick()
{
	switch (state)
	{
	case on:
		state = on;
		break;
	default:
		state = on;
		break;
	}
	switch (state)
	{
	case on:
		if (USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
		}
		break;
	default:
		break;
	}
}




int main(void) {
	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	TimerSet(25);
	TimerOn();
	initUSART(0);
	while (1) {
		Follower_Tick();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
