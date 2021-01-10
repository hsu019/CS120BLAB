/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #5
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
unsigned char password[4]={0x02,0x04,0x03,0x04};
unsigned char check[4];
unsigned char cnt;
unsigned char tmpB = 0x00;
unsigned char tmpC = 0x00;
unsigned char tmpA = 0x00;

int Check(unsigned char p[4],unsigned char c[4]){
	for (int i = 0;i<4 ;i++ )
	{
		if (p[i] != c[i])
		{
			return 0;
		}
	}
	return 1;
}

void Tick()
{
    switch (State) {
        case Start:
            State = Init;
            break;
        case Init:
            State = Wait;
            break;
        case Num_press:
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
        case X_press:
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
        case Y_press:
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
        case FI_press:
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
        case Wait:
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
			cnt = 0;
			break;
        case Num_press:
            tmpC = 0x02;
			PORTC = tmpC;
			if (cnt < 4 )
			{
				check[cnt] = tmpC;
				cnt++;
			}
            break;
        case X_press:
            tmpC = 0x04;
			PORTC = tmpC;
			if (cnt < 4 )
			{
				check[cnt] = tmpC;
				cnt++;
			}
            if (cnt == 4) {
                if (Check(password, check) == 0)
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
            }
            break;
        case Y_press:
            tmpC = 0x03;
			PORTC = tmpC;
			if (cnt < 4 )
			{
				check[cnt] = tmpC;
				cnt++;
			}
            break;
        case FI_press:
            tmpB = 0x00;
			PORTB = tmpB;
            tmpC = 0x05;
			PORTC = tmpC;
            break;
        case Wait:
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