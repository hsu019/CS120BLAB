/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 14 Exercise 3 leader
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1zh4jZSgKYxeS2BFZqDjxsdngYTXIVrGd/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "usart_ATmega1284.h"
#endif

unsigned char check;
unsigned char temp;
unsigned char led1 = 0x00;
static unsigned int period = 1000;


enum Leader_States { on, off }state;

void Leader_Tick()
{
	switch (state)
	{
	case on:
		if (!check) {
			PORTC = 0x00;
			period = 25;
			TimerSet(period);
			state = off;
		}
		else {
			PORTC = 0x01;
			if (USART_HasReceived(0)) {
				period = 25;
				TimerSet(period);
				state = off;
			}
		}
		break;
	case off:
		if (check)
		{
			PORTC = 0x01;
			period = 1000;
			TimerSet(period);
			state = on;
		}
		else {
			PORTC = 0x00;
			if (!USART_HasReceived(0)) {
				period = 1000;
				TimerSet(period);
				state = on;
			}
		}
		break;
	default:
		state = off;
		break;
	}
	switch (state)
	{
	case on:
		if (USART_IsSendReady(1))
		{
			USART_Send(led1, 1);
			PORTA = led1;
		}
		if (USART_HasTransmitted(1)) {

			led1 = ~led1;
		}
		break;
	case off:
		if (USART_HasReceived(0))
		{
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
		}
		PORTC = 0x00;
		break;
	default:
		break;
	}
}






int main(void) {
    /* Insert DDR and PORT initializations */

	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
    TimerSet(period);
    TimerOn();
	initUSART(0);
	initUSART(1);
    while (1) {
		check = (~PINB & 0x01);
		Leader_Tick();
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
