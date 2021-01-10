#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as outputs, initialize to 0s
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char total = 0x00;
while(1) {
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;

		total = tmpA + tmpB + tmpC;

		if (total > 140) { 
			tmpD = (tmpD & 0xFE) | 0x01;
		}
		PORTD = tmpD;
		if (tmpA > tmpB)
		{
			if((tmpA - tmpB) > 80){
				tmpD = (tmpD & 0xFD) | 0x02;
			}
		}
		else{
			if((tmpB - tmpA) > 80){
				tmpD = (tmpD & 0xFD) | 0x02;
			}
		}
		
		//tmpD = (tmpD & 0x03) | (total & 0xFC);
		
		//PORTD = tmpD;	
	}
    return 1;
}

/*
test "PINA: 0x00 PINB: 0x00 PINC: 0x00 => PORTD: 0x00"
setPINA 0x00
setPINB 0x00
setPINC 0x00
continue 5
expectPORTD 0x00
checkResult

test "PINA: 0x10 PINB: 0x10 PINC: 0x10 => PORTC: 0xC1"
setPINA 0x10
setPINB 0x10
setPINC 0x10
continue 5
expectPORTB 0xC1
checkResult

test "PINA: 0x11 PINB: 0x00 PINC: 0x04 => PORTC: 0x86"
setPINA 0x11
setPINB 0x00
setPINC 0x04
continue 5
expectPORTB 0x86
checkResult

*/