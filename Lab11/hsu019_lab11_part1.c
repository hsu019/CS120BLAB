/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 11 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1BdkoiAVyJ-Ao4CexXh7GC3Y8L3fC-kNK/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

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
unsigned char LEDdata = 0xFF;

enum States { Light ,increase, decrease, off_light } State;
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
    default:
        State = Light;
        break;
    }

    switch (State)
    {
    case Light:
        transmit_data(LEDdata);
        break;
    case increase:
        if (LEDdata < 0xFF) {
            LEDdata++;
        }
        break;
    case decrease:
        if (LEDdata > 0x00){
            LEDdata--;
        }
        break;
    case off_light:
        LEDdata = 0x00;
        break;
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
    TimerSet(100);
    TimerOn();
    while (1) {
        tmpA = ~PINA;
        TickFct_Blink();
        while (!TimerFlag) {}
        TimerFlag = 0;
    }
    return 0;
}
