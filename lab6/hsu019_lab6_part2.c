/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 6  Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1fzVUn3ioqUqEnZs7DQOw6XlXRYAX2vZC/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

unsigned char tmpB;
unsigned char tmpA = 0x00;

enum States { Start, Init, One, Two, Three, In_Two, Pause ,Reset} State;

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
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


void TickFct_Blink()  {
	switch (State){
		case Start:
			State = Init;
			break;
		case Init:
			State = One;
			break;
		case One:
			if (tmpA == 0x01)
			{
				State = Pause;
			}
			else {
				State = Two;
			}
			break;
		case Two:
			if (tmpA == 0x01)
			{
				State = Pause;
			}
			else {
				State = Three;
			}
			break;
		case Three:
			if (tmpA == 0x01)
			{
				State = Pause;
			}
			else {
				State = In_Two;
			}
			break;
		case In_Two:
			if (tmpA == 0x01)
			{
				State = Pause;
			}
			else {
				State = One;
			}
			break;
		case Pause:
			if(tmpA == 0x00)
			{
				State = Pause;
			}
			else {

				State = Reset;
			}
			break;
		case Reset:
			State = One;
			break;
		default:
            State = Start;
            break;
	}

	switch (State){
	case Init:
		break;
	case One:
		tmpB = 0x01;
		PORTB = tmpB;
		break;
	case Two:
		tmpB = 0x02;
		PORTB = tmpB;
		break;
	case Three:
		tmpB = 0x04;
		PORTB = tmpB;
		break;
	case In_Two:
		tmpB =0X02;
		PORTB = tmpB;
		break;
	case Pause:
		break;
	case Reset:
		break;
	default:
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(300);
    TimerOn();
    while(1){
		tmpA = ~PINA;
		TickFct_Blink(); 
		while (!TimerFlag){}
		TimerFlag = 0;
    }
    return 0;
}
