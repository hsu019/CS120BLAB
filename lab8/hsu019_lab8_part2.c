/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 8 Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1f0ZakgxpjQPyTqNcnrPJnZbkQ73IGgRC/view?usp=sharing
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
double array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
int i = 0;
enum States { Start, init,on, off, up, down, wait, process } State;

void TickFct_Blink()
{
	 switch(State) {
		 case Start:
			 State = off;
			 break;
		 case off:
			 if(tmpA == 0x01){
				State = wait;
				check = 0x00;
			 }
			 break;
		 case init:
			 State = on;
		     break;
		 case on:
			 if(tmpA == 0x01){
				State = wait;
				check = 0x01;
			 }
			 if (tmpA == 0x02)
			 {
				 State = up;
			 }
			 if (tmpA == 0x04)
			 {
				 State = down;
			 }
			 break;
	     case wait:
			 State = process;
			 break;
		 case up:
			 State = on;
			 break;
		 case down:
			 State = on;
			 break;
		 case process:
			 if(tmpA == 0x00 && check == 0x01){
				State = off;
			 }
			 if(tmpA == 0x00 && check == 0x00)
			 {
				 State = init;
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
	     break;
	 case init:
		 i = 0; 
		 set_PWM(array[i]);
		 break;
	 case wait:
	     break;
	 case up:
		 if (i !=7) 
			{
				i += 1;	
			}
			set_PWM(array[i]);
		 break;
	 case down:
		 if (i !=0) 
			{
				i -= 1;	
			}
			set_PWM(array[i]);
		 break;
	 case process:
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
