/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 11 Exercise 5
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif


/***************************************************************11.27************************/
unsigned short     int LEDdata_Q = 0;

unsigned short     int Temp_LED  = 0x8000;

void transmit_data_Q(unsigned char data)
	{
    int i;
    for (i = 0; i < 8; ++i) {
        // Sets SRCLR to 1 allowing data to be set
        // Also clears SRCLK in preparation of sending data
        PORTC = 0x08;
        // set SER = next bit of data to be sent.
        PORTC |= ((data >> i) & 0x01);
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTC |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from ¡°Shift¡± register to ¡°Storage¡± register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}












/***************************************************************11.27************************/










void transmit_data(unsigned char data) {
    int i;
    for (i = 0; i < 8; ++i) {
        // Sets SRCLR to 1 allowing data to be set
        // Also clears SRCLK in preparation of sending data
        PORTC = 0x08;
        // set SER = next bit of data to be sent.
        PORTC |= ((data >> i) & 0x01);
        // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
        PORTC |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from ¡°Shift¡± register to ¡°Storage¡± register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}

unsigned char tmpA;
 unsigned short     int LEDdata = 1;




enum States { Light ,increase, decrease, off_light,QQ,WW } State;
void TickFct_Blink() {
    switch (State)
    {
    case Light:
        if (tmpA == 0x01)
        {
            State = increase;
        }
        else if (tmpA == 0x02)
        {
            State = decrease;
        }
        else if (tmpA == 0x03) {
            State = off_light;
        }
/***************************************************1127*************************************/	

		else if (tmpA == 0x05)		//ÓÒ  Ìø
		{
            State = QQ;
        }
		else if (tmpA == 0x06)		//×ó  Ìø
		{
            State = WW;
        }
		
/***************************************************1127*************************************/			
        else
        {
            State = Light;
        }
        break;
    case increase:
        State = Light;
        break;
    case decrease:
        State = Light;
        break;
    case off_light:
        State = Light;
        break;
    case QQ:
        State = Light;
        break;	
	
	    case WW:
        State = Light;
        break;	
	
    default:
        State = Light;
        break;
    }

    switch (State)
    {
    case Light:
        transmit_data(LEDdata);
	
/***************************************************1127*************************************/
	
		LEDdata_Q = LEDdata >>8;
		transmit_data_Q(LEDdata_Q);	//add
	
	
/***************************************************1127*************************************/		
	
	
        break;
    case increase:
		
	
	
        if (LEDdata < 0xc000) 
		{
          LEDdata =  LEDdata<<1;
        }
	
		
		
		
/***************************************************1127*************************************/			
		
        break;
    case decrease:
		
        if (LEDdata > 1) 
		{
          LEDdata =  LEDdata >> 1;
        }
		
        break;
    case off_light:
        LEDdata = 0x00;
	
	
        break;
	
	
	    case QQ:
		if (LEDdata < 0x8000) 
		{
          LEDdata =  LEDdata<<4;
        }	

		
		
		
		
	
        break;	
	
	    case WW:
			
		if (LEDdata > 1) 
		{
          LEDdata =  LEDdata >>4;
        }	

	
	
	
	
    default:
        break;
    }

 }




int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRC = 0xFF;
    PORTC = 0x00;
    TimerSet(1000);
    TimerOn();
    while (1) {
        tmpA = ~PINA;
        TickFct_Blink();
        while (!TimerFlag) {}
        TimerFlag = 0;
			
			

			if( Temp_LED > 1 )
			{
				Temp_LED = 	Temp_LED >>1;
				
			}
			else
			{
				Temp_LED = 0x8000;

			}
			if(Temp_LED == LEDdata)
			{
				LEDdata = 0;
				Temp_LED = 0;
			
			}
			else
			{
				LEDdata = Temp_LED | LEDdata;
			}				

			
			
    }
    return 0;
}
