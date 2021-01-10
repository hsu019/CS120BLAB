/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 10 Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1hDk7qkEgm21utdtrKBN84uH3Sdw0rgv3/view?usp=sharing
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

int Check(int p[5], int c[5]) {
    for (int i = 0; i < 5; i++)
    {
        if (p[i] != c[i])
        {
            return 0;
        }
    }
    return 1;
}

enum pauseButtonSM_States {wait,num_press, one_press,two_press, three_press, four_press, five_press, release, lock};


int password[5] = {1,2,3,4,5};
int check[5];
unsigned char cnt;
unsigned num_pressed = 0;
unsigned char tmpB;
unsigned char inputB;

int pauseButtonSMTick(int state) {
    inputB = ~PINB & 0x80;
    unsigned char x = GetKeypadKey();
    switch (state)
    {
    case wait: 
        if (x == '#')
        {
            state = num_press;
        }
        else if (x == '1')
        {
            state = one_press;
        }
        else if (x == '2') {
            state = two_press;
        }
        else if (x == '3') {
            state = three_press;
        }
        else if (x == '4') {
            state = four_press;
        }
        else if (x == '5') {
            state = five_press;
        }
        else if (inputB == 0x80) {
            if (tmpB == 0x01) {
                state = lock;
            }
        }
        else
        {
            state = wait;
        }
        break;
    case num_press:
        state = release;
        break;
    case one_press:
        state = release;
        break;
    case two_press:
        state = release;
        break;
    case three_press:
        state = release;
        break;
    case four_press:
        state = release;
        break;
    case five_press:
        state = release;
        break;
    case lock:
        state = wait;
        break;
    case release:
        if (x == '\0') {
            state = wait;
        }
        break;
    default:
        break;
    }
    
    switch (state) {
    case wait:
       if (cnt == 5) {
            if (Check(password, check) == 1) {
                tmpB = 0x01;
                PORTB = tmpB;
            }
        }
        break;
    case release:
        break;
    case lock:
        num_pressed = 0;
        cnt = 0;
        tmpB = 0x00;
        PORTB = 0x00;
        break;
    case num_press:
        num_pressed = 1;
        cnt = 0;
        break;
    case one_press:
        if (num_pressed == 1) {
            if (cnt < 5)
            {
                check[cnt] = 1;
                cnt++;
            }
        }
        break;
    case two_press:
        if (num_pressed == 1) {
            if (cnt < 5)
            {
                check[cnt] = 2;
                cnt++;
            }
        }
        break;
    case three_press:
        if (num_pressed == 1) {
            if (cnt < 5)
            {
                check[cnt] = 3;
                cnt++;
            }
        }
        break;
    case four_press:
        if (num_pressed == 1) {
            if (cnt < 5)
            {
                check[cnt] = 4;
                cnt++;
            }
        }
        break;
    case five_press:
        if (num_pressed == 1) {
            if (cnt < 5)
            {
                check[cnt] = 5;
                cnt++;
            }
        }
        break;
    default:
        break;
    }
    return state;
}



int main(void) {
    DDRB = 0x0F; PORTB = 0xF0;
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