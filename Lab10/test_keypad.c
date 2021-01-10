/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 10 Exercise 4
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
#include "bit.h"
#endif

unsigned char check = 0;

unsigned char GetKeypadKey() {

        PORTC = 0xEF;
        asm("nop");
        if (GetBit(PINC, 0) == 0) { return ('1'); }
        if (GetBit(PINC, 1) == 0) { return ('4'); }
        if (GetBit(PINC, 2) == 0) { return ('7'); }
        if (check == 0) {
            if (GetBit(PINC, 3) == 0)
            {
                check = 1;
                return ('*');
            }
        }
        else {
            PORTC = 0xBF;
            if (GetBit(PINC, 2) == 0) { return ('o'); }

        }
        
        //if (GetBit(PINC, 3) == 0 && GetBit(PINC, 1) == 0) {return ('o'); }

        PORTC = 0xDF;
        asm("nop");
        if (GetBit(PINC, 0) == 0) { return ('2'); }
        if (GetBit(PINC, 1) == 0) { return ('5'); }
        if (GetBit(PINC, 2) == 0) { return ('8'); }
        if (GetBit(PINC, 3) == 0) { return ('0'); }

        PORTC = 0xBF;
        asm("nop");
        if (GetBit(PINC, 0) == 0) { return ('3'); }
        if (GetBit(PINC, 1) == 0) { return ('6'); }
        if (GetBit(PINC, 2) == 0) { return ('9'); }
        if (GetBit(PINC, 3) == 0) { return ('#'); }

        PORTC = 0x7F;
        asm("nop");
        if (GetBit(PINC, 0) == 0) { return ('A'); }
        if (GetBit(PINC, 1) == 0) { return ('B'); }
        if (GetBit(PINC, 2) == 0) { return ('C'); }
        if (GetBit(PINC, 3) == 0) { return ('D'); }

  
    return ('\0');
}



int main(void) {
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;
    
    unsigned char x; 
    while (1) {
        x = GetKeypadKey();
        switch (x)
        {
        case '\0': PORTB = 0x1F; break;
        case '1': PORTB = 0x01; break;
        case '2': PORTB = 0x02; break;
        case '3': PORTB = 0x03; break;
        case '4': PORTB = 0x04; break;
        case '5': PORTB = 0x05; break;
        case '6': PORTB = 0x06; break;
        case '7': PORTB = 0x07; break;
        case '8': PORTB = 0x08; break;
        case '9': PORTB = 0x09; break;
        case 'A': PORTB = 0x0A; break;
        case 'B': PORTB = 0x0B; break;
        case 'C': PORTB = 0x0C; break;
        case 'D': PORTB = 0x0D; break;
        case '*': PORTB = 0x0E; break;
        case '0': PORTB = 0x00; break;
        case '#': PORTB = 0x0F; break;
        case 'o': PORTB = 0x11; break;
        default:
            PORTB = 0x1B;
            break;
        }
    }
    return 0;
}