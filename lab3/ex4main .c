#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; 
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; 
	unsigned char tmpA = 0x00; 
	unsigned char upper;
	unsigned char lower;
while(1) {
		tmpA = PINA;
		upper = 0x00;
		lower = 0x00;

		lower = (tmpA & 0xF0) >> 4;
		upper = (tmpA & 0x0F) << 4;

		PORTB = lower;
		PORTC = upper;

	}
	return 0;
}

/*
tests = [ {'description': 'This test will run first.',
    'steps': [ {'inputs': [('PINA',0x70)], 'iterations': 1 } ],
    'expected': [('PORTB',0x07),('PORTC',0x00)],
    },
    {'description': 'This test will run second.',
    'steps': [ 
        {'inputs': [('PINA',0x05)], 'iterations': 1}, ],
    'expected': [('PORTB',0x00),('PORTC',0x50)],
    },
    {'description': 'This is test will run third.',
    'steps': [ {'inputs': [('PINA',0xFC)], 'iterations': 1 } ],
    'expected': [('PORTB',0x0F),('PORTC',0xC0)],
    },

    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint
watch = ['main::tmpA','PORTB,PORTC']

*/