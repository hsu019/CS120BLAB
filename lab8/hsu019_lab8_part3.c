/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 8 Exercise 3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1aeNlwIkIW4z1t_YRh8Gg-7QvaRKq2cbl/view?usp=sharing
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

void set_PWM(double frequency) {
	static double current_frequency;
	if(frequency != current_frequency) {
		if (!frequency)
		{
			TCCR3B &= 0x08;
		}
		else {
			TCCR3B |= 0x03;
		}

		if (frequency < 0.954)
		{
			OCR3A = 0xFFFF;
		}
		else if (frequency > 31250)
		{
			OCR3A = 0x0000;
		}
		else {
			OCR3A = (short)(8000000 / (128 * frequency)) - 1;
		}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char tmpA = 0x00;
unsigned char check = 0x00;
double array[10] = {493.88,293.66, 493.88, 293.66,493.88,493.88,493.88,329.63,293.66,261.63};
int i = 0;
int count = 0;
enum States { Start, on, off, wait } State;

void TickFct_Blink()
{
	 switch(State) {
		 case Start:
			 State = off;
			 break;
		 case off:
			if (tmpA == 0x01) //  on
			{
				i = 0; 
				count = 0;
				State = on;
			}
			break;
		 case on:
			 if (count == 30)
			 {
			 	State = wait;
			 }
			 break;
	     case wait:
			 if (tmpA == 0x00)
			 {
			 	State = off;
			 }
			 break;
		 default:
			 State = Start;
			 break;
	}

	 switch(State) {
	 case off:
		 set_PWM(0.0);
	 	 break;
	 case on:
		 if (count == 0)
		 {
			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 2){
			set_PWM(0.00);
		 }
		 else if (count == 3)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 5){
			set_PWM(0.00);
		 }
		 else if (count == 6)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 8){
			set_PWM(0.00);
		 }
		 else if (count == 9)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 12){
			set_PWM(0.00);
		 }
		 else if (count == 13)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 14){
			set_PWM(0.00);
		 }
		 else if (count == 15)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 16){
			set_PWM(0.00);
		 }
		 else if (count == 17)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 18){
			set_PWM(0.00);
		 }
		 else if (count == 19)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 21){
			set_PWM(0.00);
		 }
		 else if (count == 22)
		 {

			 set_PWM(array[i]);
			 i++;
		 }
		 else if(count == 25){
			set_PWM(0.00);
		 }
		 else if (count == 26)
		 {
			 set_PWM(array[i]);
			 i++;

		 }

	     break;
	 case wait:
		 set_PWM(0.0);
	     break;
	 default:
		 break;
	 }

	 count++;
}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;
	PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
	PWM_on();
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
