/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 9 Exercise 1
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
#endif

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

unsigned char BL_tmpB;
enum BL_States {BL_Start, BL_Init, On, Off} BL_State;

void TickFct_BlinkLed(){
	switch (BL_State){
		case BL_Start:
			BL_State = BL_Init;
			break;
		case BL_Init:
			BL_State = On;
			break;
		case On:
			BL_State = Off;
			break;
		case Off:
			BL_State = On;
			break;
		default:
            BL_State = BL_Start;
            break;
	}

	switch (BL_State){
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
}


unsigned char TL_tmpB;
enum TL_States { TL_Start, TL_Init, One, Two, Three } TL_State;
int i;

void TickFct_ThreeLeds()  {
	switch (TL_State){
		case TL_Start:
			TL_State = TL_Init;
			break;
		case TL_Init:
			TL_State = One;
			break;
		case One:
			TL_State = Two;
			break;
		case Two:
			TL_State = Three;
			break;
		case Three:
			TL_State = One;
			break;
		default:
            TL_State = TL_Start;
            break;
	}

	switch (TL_State){
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
}

unsigned char tmpB;
enum CB_States {CB_Start, CB} CB_State;
int j;

void TickFct_Combine_State(){
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
    TimerSet(100);
    TimerOn();
	BL_State = BL_Start;
    TL_State = TL_Start; 
    while(1){
		TickFct_BlinkLed();
		TickFct_ThreeLeds();
		TickFct_Combine_State();
		while (!TimerFlag){}
		TimerFlag = 0;
    }
    return 0;
}
