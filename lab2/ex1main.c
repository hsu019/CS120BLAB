#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
while(1) {
		tmpA = PINA;
		if (tmpA == 0x01) { 
			tmpB = (tmpB & 0xFE) | 0x01; 
		} else {
			tmpB = (tmpB & 0xFE); 
		}	
	PORTB = tmpB;	
	}
	return 0;
}

/*
test "PINA: 0x00 => PORTB: 0x00"
setPINA 0x00
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x02 => PORTB: 0x00"
setPINA 0x02
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x01 => PORTB: 0x01"
setPINA 0x01
continue 5
expectPORTB 0x01
checkResult

*/