/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 14 Exercise 1 Servant
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1Oa7QrpPtNHDIZcW_4VIkjxtaR_4Rp7D7/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#endif

unsigned char receivedData;

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

enum servantStates { s_wait, read } states;
void TickFct_servant()
{
	switch (states)
	{
	case s_wait:
		states = read;
		break;
	case read:
		states = s_wait;
		break;
	default:
		states = s_wait;
		break;
	}
	switch (states)
	{
	case s_wait:
		break;
	case read:
		PORTA = receivedData;
		break;
	default:
		break;
	}
}




int main(void) {
	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	SPI_ServantInit();
	TimerSet(100);
	TimerOn();
	while (1) {
		TickFct_servant();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
