/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 14 Exercise 1 leader
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

unsigned char led1 = 0x00;
enum Leader_States { on } state;
void Leader_Tick()
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
		if (USART_IsSendReady(0))
		{
			USART_Send(led1, 0);
			PORTA = led1;

		}
		if (USART_HasTransmitted(0)) {
			led1 = ~led1;
		}


		break;

	default:
		break;
	}
}




int main(void) {
    /* Insert DDR and PORT initializations */

	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
    TimerSet(1000);
    TimerOn();
	initUSART(0);
    while (1) {
		Leader_Tick();
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
