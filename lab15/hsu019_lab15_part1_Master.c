/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 15 Exercise 1 Master
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
#include "keypad.h"
#include "bit.h"
#endif


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
enum masterStates { wait, send } state;
void TickFct_master()
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
		if (data == 0xFF) {
			data = 0x00;
		}
		else {
			data++;
		}
		break;
	case send:
		SPI_MasterTransmit(data);
		break;
	default:
		break;
	}
}




int main(void) {
	/* Insert DDR and PORT initializations */

	// initialize ports
	DDRA = 0xF0; PORTA = 0x0F;
	SPI_MasterInit();
	TimerSet(1000);
	TimerOn();
	while (1) {
		TickFct_master();
		while (!TimerFlag) {}
		TimerFlag = 0;
	}
	return 0;
}
