/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 10 Exercise 3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/14OrtztPOqzhcupurfgeWWl7r-TPFweHH/view?usp=sharing
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

void set_PWM(double frequency) {
    static double current_frequency;
    if (frequency != current_frequency) {
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

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
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
            if ((tmpB & 0x01)== 0x01) {
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
                PORTB = tmpB | inputB;
            }
        }
        break;
    case release:
        break;
    case lock:
        num_pressed = 0;
        cnt = 0;
        tmpB = 0x00;
        PORTB = 0x00 | inputB;
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

unsigned char tmpA = 0x00;
double array[10] = { 493.88,293.66, 493.88, 293.66,493.88,493.88,493.88,329.63,293.66,261.63 };
int j = 0;
int count = 0;
enum States { Start, on, off, wait_speaker };

int TickFct_Blink(int state)
{
    tmpA = ~PINA;
    switch (state) {
    case Start:
        state = off;
        break;
    case off:
        if (tmpA == 0x80) //  on
        {
            j = 0;
            count = 0;
            state = on;
        }
        break;
    case on:
        if (count == 30)
        {
            state = wait_speaker;
        }
        break;
    case wait_speaker:
        if (tmpA == 0x00)
        {
            state = off;
        }
        break;
    default:
        state = Start;
        break;
    }

    switch (state) {
    case off:
        set_PWM(0.0);
        break;
    case on:
        if (count == 0)
        {
            set_PWM(array[j]);
            j++;
        }
        else if (count == 2) {
            set_PWM(0.00);
        }
        else if (count == 3)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 5) {
            set_PWM(0.00);
        }
        else if (count == 6)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 8) {
            set_PWM(0.00);
        }
        else if (count == 9)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 12) {
            set_PWM(0.00);
        }
        else if (count == 13)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 14) {
            set_PWM(0.00);
        }
        else if (count == 15)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 16) {
            set_PWM(0.00);
        }
        else if (count == 17)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 18) {
            set_PWM(0.00);
        }
        else if (count == 19)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 21) {
            set_PWM(0.00);
        }
        else if (count == 22)
        {

            set_PWM(array[j]);
            j++;
        }
        else if (count == 25) {
            set_PWM(0.00);
        }
        else if (count == 26)
        {
            set_PWM(array[j]);
            j++;

        }
        
        break;
    case wait_speaker:
        set_PWM(0.0);

        break;
    default:

        break;
    }

    count++;
    return state;
}


int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;

    PWM_on();


    static task task1, task2;
    task* tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = 0;//Task initial state.
    task1.period = 100;//Task Period.
    task1.elapsedTime = task1.period;//Task current elapsed time.
    task1.TickFct = &pauseButtonSMTick;//Function pointer for the tick.

    task2.state = 0;//Task initial state.
    task2.period = 100;//Task Period.
    task2.elapsedTime = task2.period;//Task current elapsed time.
    task2.TickFct = &TickFct_Blink;//Function pointer for the tick.

    TimerSet(100);
    TimerOn();
    
    unsigned short i; 
    while (1) {
        for (i = 0; i < numTasks; i++) {
            if (tasks[i]->elapsedTime == tasks[i]->period) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 100;
        }
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 0;
}