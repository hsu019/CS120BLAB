/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 6  Exercise 3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1_pG0Pi8PT_LrLeuMLP8V30wCZO4FhOAI/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States { Start, Init, Wait, Increase, Decrease, Reset } State;
unsigned char count = 0x07;
unsigned char tmpA = 0x00;



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

void TickFct_Blink()
{
	 switch(State) {
		 case Start:
			 State = Wait;
			 break;
		 case Increase:
			 State = Wait;
			 break;
		 case Decrease:
			 State = Wait;
			 break;
		 case Reset:
			 State = Wait;
			 break;
		 case Wait:
			 if (tmpA == 0x01) {
			 	State = Increase;
			 }
			 else if (tmpA == 0x02) {
				State = Decrease;
			 }
			 else if (tmpA == 0x03)
			 {
				 State = Reset;
			 }
			 else if (tmpA == 0x00)
			 {
				 State = Wait;
			 }
			 else {
				State = Wait;
			 }
			 break;
		 default:
			 State = Start;
			 break;
	}

	 switch(State) {
	 case Increase:
		 if (count < 0x09)
		 {
		 	count++;
	 		PORTB = count;
		 }
		 else {
			PORTB = count;
		 }
	 	 break;
	 case Decrease:
		 if (count > 0x00)
		 {
		 	count = count - 1;
	 		PORTB = count;
		 }
		 else {
			PORTB = count;
		 }
	     break;
	 case Reset:
		 count = 0x00;
	 	 PORTB = count;
	     break;
	 case Wait:
	 	 PORTB = count;
	     break;
	 default:
		 break;
	 }
	}



int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0xFF;PORTB = 0x00;
	State = Start; 
	TimerSet(100);
    TimerOn();
    while(1){
		tmpA = ~PINA;
		TickFct_Blink(); 
		while (!TimerFlag){}
		TimerFlag = 0;
    }
    return 0;
}
