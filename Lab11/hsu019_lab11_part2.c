/* Author: Huapei Su
 * Partner(s) Name (if applicable):  none
 * Lab Section:023
 * Assignment: Lab 11 Exercise 2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: GOOGLE DRIVE https://drive.google.com/file/d/1ZX5YZq77fVM_QDCmEuijIIcoX5LAZuyB/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

typedef struct task {
    int state; // Current state of the task
    unsigned long period; // Rate at which the task should tick
    unsigned long elapsedTime; // Time since task's previous tick
    int (*TickFct)(int); // Function to call for task's tick
} task;



unsigned char go = 0x01;

unsigned char tmpA;
unsigned char LEDdata_A = 0x01;
unsigned char LEDdata_B = 0x00;
unsigned char LEDdata_C;

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


enum BL_A_States { Light_A_up, Light_A_down };

int TickFct_BlinkLed_A(int state) {
    switch (state)
    {
    case Light_A_up:

            if (LEDdata_A == 0x80)
            {
                state = Light_A_down;
            }
            else {
                state = Light_A_up;
            }
        

        break;
    case Light_A_down:

            if (LEDdata_A == 0x01)
            {
                state = Light_A_up;
            }
            else {
                state = Light_A_down;
            }


        break;

    default:
        state = Light_A_up;
        break;
    }

    switch (state)
    {
    case Light_A_up:
        if (go == 0x01) {
            LEDdata_A = LEDdata_A << 1;
            transmit_data(LEDdata_A);
        }
        break;
    case Light_A_down:
        if (go == 0x01) {
            LEDdata_A = LEDdata_A >> 1;
            transmit_data(LEDdata_A);
        }
        break;
    default:
        break;
    }
    return state;
 }


enum BL_B_States { Light_B_up,Light_B_down };

int TickFct_BlinkLed_B(int state) {
    switch (state)
    {
    case Light_B_up:
        if (LEDdata_B == 0xFF)
        {
            state = Light_B_down;
        }
        else
        {
            state = Light_B_up;
        }
        break;
    case Light_B_down:
        if (LEDdata_B == 0x00)
        {
            state = Light_B_up;
        }
        else
        {
            state = Light_B_down;
        }
        break;
    default:
        break;
    }

    switch (state)
    {
    case Light_B_up:
        if (go == 0x02) {
            LEDdata_B = (LEDdata_B << 1) | 0x01;
            transmit_data(LEDdata_B);
        }
        break;
    case Light_B_down:
        if (go == 0x02) {
            LEDdata_B = (LEDdata_B >> 1);
            transmit_data(LEDdata_B);
        }
        break;

    default:
        break;
    }

    return state;
}

unsigned char check;
enum BL_C_States { Light_C_zero, Light_C_one, Light_C_two, Light_C_three, Light_C_four};

int TickFct_BlinkLed_C(int state) {
    switch (state)
    {
    case Light_C_zero:
            check = 0x00;
            state = Light_C_one;
        break;
    case Light_C_one:
        if (check == 0x00) {
            state = Light_C_two;
        }
        if (check == 0x01)
        {
            state = Light_C_zero;
        }
        break;
    case Light_C_two:
        if (check == 0x00) {
            state = Light_C_three;
        }
        if (check == 0x01)
        {
            state = Light_C_one;
        }
        break;
    case Light_C_three:
        if (check == 0x00) {
            state = Light_C_four;
        }
        if (check == 0x01)
        {
            state = Light_C_two;
        }
        break;
    case Light_C_four:
            check = 0x01;
            state = Light_C_three;
        break;

    default:
        break;
    }
    
    switch (state)
    {
    case Light_C_zero:
        if (go == 0x03) {
            LEDdata_C = 0x00;
            transmit_data(LEDdata_C);
        }
        break;
    case Light_C_one:
        if (go == 0x03) {
            LEDdata_C = 0x81;
            transmit_data(LEDdata_C);
        }
        break;
    case Light_C_two:
        if (go == 0x03) {
            LEDdata_C = 0xC3;
            transmit_data(LEDdata_C);
        }
        break;
    case Light_C_three:
        if (go == 0x03) {
            LEDdata_C = 0xE7;
            transmit_data(LEDdata_C);
        }
        break;
    case Light_C_four:
        if (go == 0x03) {
            LEDdata_C = 0xFF;
            transmit_data(LEDdata_C);
        }
        break;
    default:
        break;
    }

    return state;
}


enum CB_States { CB_off,CB_on ,CB_increase,CB_decrease,CB_init};

int TickFct_Combine_State(int state) {
    switch (state) {
    case CB_init:
        state = CB_on;
        break;
    case CB_on:
        if (tmpA == 0x01)
        {
            state = CB_increase;
        }
        else if (tmpA == 0x02)
        {
            state = CB_decrease;
        }
        else if (tmpA == 0x03) {
            state = CB_off;
        }
        else
        {
            state = CB_on;
        }
        break;
    case CB_increase:
        state = CB_on;
        break;
    case CB_decrease:
        state = CB_on;
        break;
    case CB_off:
        if (tmpA == 0x03)
        {
            state = CB_init;
        }
        else
        {
            state = CB_off;
        }
        break;
    default:
        state = CB_off;
        break;
    }
    switch (state) {
    case CB_init:
        go = 0x01;
        break;
    case CB_on:
        break;
    case CB_increase:
        if (go < 0x03)
        {
            go++;
        }
        break;
    case CB_decrease:
        if (go > 0x01)
        {
            go--;
        }
        break;
    case CB_off:
        go = 0x00;
        transmit_data(0x00);
        break;
    default:
        break;
    }
    return state;
}




int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRC = 0xFF;
    PORTC = 0x00;

    static task task1, task2,task3,task4;
    task* tasks[] = { &task1, &task2 ,&task3,&task4};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

    task1.state = 0;//Task initial state.
    task1.period = 100;//Task Period.
    task1.elapsedTime = task1.period;//Task current elapsed time.
    task1.TickFct = &TickFct_BlinkLed_A;//Function pointer for the tick.

    task2.state = 0;//Task initial state.
    task2.period = 100;//Task Period.
    task2.elapsedTime = task2.period;//Task current elapsed time.
    task2.TickFct = &TickFct_BlinkLed_B;//Function pointer for the tick.

    task3.state = 0;//Task initial state.
    task3.period = 100;//Task Period.
    task3.elapsedTime = task3.period;//Task current elapsed time.
    task3.TickFct = &TickFct_BlinkLed_C;//Function pointer for the tick.

    task4.state = 0;//Task initial state.
    task4.period = 100;//Task Period.
    task4.elapsedTime = task4.period;//Task current elapsed time.
    task4.TickFct = &TickFct_Combine_State;//Function pointer for the tick.

    TimerSet(100);
    TimerOn();

    unsigned short i;
    while (1) {
        tmpA = ~PINA;
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
