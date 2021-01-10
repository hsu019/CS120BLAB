/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 10 Exercise 1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE  no demo required 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "keypad.h"
#endif

typedef struct task {
	signed char state; // Current state of the task
	unsigned long int period; // Rate at which the task should tick
	unsigned long int elapsedTime; // Time since task's previous tick
	int (*TickFct)(int); // Function to call for task's tick
} task;

unsigned char tmpB = 0x00;
enum pauseButtonSM_States {press_pad};

int pauseButtonSMTick(int state) {
    unsigned char x = GetKeypadKey();
    switch (state)
    {
    case press_pad: 
        state = press_pad;
        break;
    default:
        break;
    }
    
    switch (state) {
    case press_pad:
        switch (x)
        {
        case '\0':
            tmpB = 0x1F;
            break;
        case '1':
            tmpB = 0x81;
            break;
        case '2':
            tmpB = 0x82;
            break;
        case '3':
            tmpB = 0x83;
            break;
        case '4':
            tmpB = 0x84;
            break;
        case '5':
            tmpB = 0x85;
            break;
        case '6':
            tmpB = 0x86;
            break;
        case '7':
            tmpB = 0x87;
            break;
        case '8':
            tmpB = 0x88;
            break;
        case '9':
            tmpB = 0x89;
            break;
        case 'A':
            tmpB = 0x8A;
            break;
        case 'B':
            tmpB = 0x8B;
            break;
        case 'C':
            tmpB = 0x8C;
            break;
        case 'D':
            tmpB = 0x8D;
            break;
        case '*':
            tmpB = 0x8E;
            break;
        case '0':
            tmpB = 0x80;
            break;
        case '#':
            tmpB = 0x8F;
            break;
        default:
            tmpB = 0x1B;
            break;
        }
        PORTB = tmpB;
        break;
    default:
        break;
    }
    return state;
}



int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;



    static task task1;
    task* tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = 0;//Task initial state.
    task1.period = 50;//Task Period.
    task1.elapsedTime = task1.period;//Task current elapsed time.
    task1.TickFct = &pauseButtonSMTick;//Function pointer for the tick.

    TimerSet(50);
    TimerOn();
    
    unsigned short i; 
    while (1) {
        for (i = 0; i < numTasks; i++) {
            if (tasks[i]->elapsedTime == tasks[i]->period) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 50;
        }
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}