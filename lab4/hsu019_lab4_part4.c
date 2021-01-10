/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


enum States {Start, Init, Wait, Num_press, Y_press, X_press, FI_press } State;
unsigned char count;
unsigned char tmpA = 0x00;
unsigned char tmpC;
unsigned char tmpB;
unsigned char check = 0;
void Tick()
{
    switch (State) {
        case Start:
            State = Init;
            break;
        case Init: //tmpC = 0x00;
            State = Wait;
            break;
        case Num_press: //tmpC = 0x02;
            if(tmpA == 0x04){
                State = Num_press;
            }
            else if(tmpA == 0x02){
                State = Y_press;
            }
            else if(tmpA == 0x01){
                State = X_press;
            }
            else if(tmpA == 0x80){
                State = FI_press;
            }
            else{
                State = Wait;
            }
            break;
        case X_press: //tmpC = 0x04;
            if(tmpA == 0x04){
               State = Num_press;
           }
           else if(tmpA == 0x02){
               State = Y_press;
           }
           else if(tmpA == 0x01){
               State = X_press;
           }
           else if(tmpA == 0x80){
               State = FI_press;
           }
           else{
               State = Wait;
           }
            break;
        case Y_press: //tmpC = 0x03;
            if(tmpA == 0x04){
                State = Num_press;
            }
            else if(tmpA == 0x02){
                State = Y_press;
            }
            else if(tmpA == 0x01){
                State = X_press;
            }
            else if(tmpA == 0x80){
                State = FI_press;
            }
            else{
                State = Wait;
            }
            break;
        case FI_press: //tmpC = 0x05;
            if(tmpA == 0x04){
                State = Num_press;
            }
            else if(tmpA == 0x02){
                State = Y_press;
            }
            else if(tmpA == 0x01){
                State = X_press;
            }
            else if(tmpA == 0x80 ){
                State = FI_press;
            }
            else{
                State = Wait;
            }
            break;
        case Wait: //tmpC = 0x01;
            if(tmpA == 0x04){
                State = Num_press;
            }
            else if(tmpA == 0x02){
                State = Y_press;
            }
            else if(tmpA == 0x01){
                State = X_press;
            }
            else if(tmpA == 0x80){
                State = FI_press;
            }
            else{
                State = Wait;
            }
            break;
        default:
            State = Start;
            break;
    }

		switch(State) {
		case Init:
			tmpB = 0x00;
			PORTB = tmpB;
			tmpC = 0x00;
			PORTC = tmpC;
			break;
        case Num_press:
            tmpC = 0x02;
			PORTC = tmpC;
			check = 1;
            break;
        case X_press:
            tmpC = 0x04;
			PORTC = tmpC;
			check = 1;
            break;
        case Y_press:
			if (tmpC == 0x02 && check == 0)
			{
				if (tmpB == 0x01)
				{
					tmpB = 0x00;
					PORTB = tmpB;
				}
				else{
					tmpB = 0x01;
					PORTB = tmpB;
				}
			}
            tmpC = 0x03;
			PORTC = tmpC;
			check = 1;
            break;
        case FI_press:
            tmpB = 0x00;
			PORTB = tmpB;
            tmpC = 0x05;
			PORTC = tmpC;
			check = 1;
            break;
        case Wait:
			check = 0;
            break;
        default:
            break;
	 }
	}



int main(void) {
    /* Insert DDR and PORT Initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF;PORTC = 0x00;
	
	State = Start; 
    while(1){
	tmpA = PINA;
        Tick();
    }
    return 0;
}