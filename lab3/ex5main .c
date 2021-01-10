#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	

int main(void) {
	DDRD = 0x00; PORTD = 0xFF; 
	DDRB = 0xFE;PORTB = 0x01;
	unsigned int tmpD = 0x00; 
	unsigned int tmpB = 0x00;
while(1) {
		tmpD = PIND << 1;
		tmpB = tmpD + (PINB & 0x01);

		if(tmpB >=70){
            PORTB = 0x02;
        }
        else if(tmpB>5 && tmpB<70){
            PORTB = 0x04;
        }
        else{
            PORTB = 0;
        }
	}
	return 0;
}

/*
tests = [ {'description': 'This test will run first.',
    'steps': [ {'inputs': [('PIND',0x80),('PINB',0x01)], 'iterations': 1 } ],
    'expected': [('PORTB',0x02)],
    },
    {'description': 'This test will run second.',
    'steps': [ 
        {'inputs': [('PIND',0x21),('PINB',0x00)], 'iterations': 1}, ],
    'expected': [('PORTB',0x04)],
    },
    {'description': 'This is test will run third.',
    'steps': [ {'inputs': [('PIND',0x01),('PINB',0x01)], 'iterations': 1 } ],
    'expected': [('PORTB',0x00)],
    },

    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tmpD,tmpB','PORTB']

*/