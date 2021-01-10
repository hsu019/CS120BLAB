/*	Author:  hsu019 Huapei Su
 *  Partner(s) Name: none
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  demo video link: https://drive.google.com/file/d/17yxQYRo5GrcD0_pP4bUtlH674Pal92pr/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


enum States { Start, zero, one, two, three, four, five, six, inv_five, inv_four, inv_three, inv_two, inv_one, wait, process } State;
unsigned char tmpA = 0x00;
unsigned char tmpC = 0x00;
unsigned char in_order = 0x00;


void Tick()
{
	 switch(State) {
		 case Start:
			 State = zero;
			 break;
		 case zero:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case one:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case two:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case three:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case four:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case five:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case six:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case inv_five:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case inv_four:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case inv_three:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case inv_two:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;
		case inv_one:
			 if (tmpA == 0x01)
			 {
				State = wait;
			 }
		 	 break;


		case wait:
			if(tmpA == 0x01)
			{
				State = wait;
			}
			else {
				State = process;
			}
			break;
		case process:
			if(tmpC == 0x00){
				State = one;
			}
			if(tmpC == 0x01 && in_order == 0x01){
				State = two;
			}
			if(tmpC == 0x02 && in_order == 0x01){
				State = three;
			}
			if(tmpC == 0x04 && in_order == 0x01){
				State = four;
			}
			if(tmpC == 0x08 && in_order == 0x01){
				State = five;
			}
			if(tmpC == 0x10 && in_order == 0x01){
				State = six;
			}
			if(tmpC == 0x20){
				State = inv_five;
			}
			if(tmpC == 0x10 && in_order == 0x00){
				State = inv_four;
			}
			if(tmpC == 0x08 && in_order == 0x00){
				State = inv_three;
			}
			if(tmpC == 0x04 && in_order == 0x00){
				State = inv_two;
			}
			if(tmpC == 0x02 && in_order == 0x00) {
				State = inv_one;
			}
			if (tmpC == 0x01 && in_order == 0x00)
			{
				State = zero;
			}
			break;
		 default:
			 State = Start;
			 break;
	}

	 switch(State) {
	 case zero:
	 	 tmpC = 0x00;
	 	 PORTC = tmpC;
	 	 in_order = 0x01;
		 break;
	 case one:
	 	 tmpC = 0x01;
	     PORTC = tmpC;
	 	 in_order = 0x01;
	     break;
	case two:
	 	 tmpC = 0x02;
	     PORTC = tmpC;
		 in_order = 0x01;
	     break;
	case three:
	 	 tmpC = 0x04;
	     PORTC = tmpC;
		 in_order = 0x01;
	     break;
	case four:
	 	 tmpC = 0x08;
	     PORTC = tmpC;
		 in_order = 0x01;
	     break;
	case five:
	 	 tmpC = 0x10;
	     PORTC = tmpC;
		 in_order = 0x01;
	     break;
	case six:
	 	 tmpC = 0x20;
	     PORTC = tmpC;
		 in_order = 0x00;
	     break;
	case inv_five:
	 	 tmpC = 0x10;
	     PORTC = tmpC;
		 in_order = 0x00;
	     break;
	case inv_four:
	 	 tmpC = 0x08;
	     PORTC = tmpC;
		 in_order = 0x00;
	     break;
	case inv_three:
		 tmpC = 0x04;
		 PORTC = tmpC;
		 in_order = 0x00;
		 break;
   case inv_two:
	     tmpC = 0x02;
   		 PORTC = tmpC;
		 in_order = 0x00;
		 break;
	case inv_one:
	     tmpC = 0x01;
   		 PORTC = tmpC;
		 in_order = 0x00;
		 break;


	case wait:
		break;
	case process:
		break;
	 default:
		 break;
	 }
	}



int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;PORTA = 0xFF;
    DDRC = 0xFF;PORTC = 0x00;
	State = Start; 
    while(1){
		tmpA = ~PINA;
        Tick();
    }
    return 0;
}
