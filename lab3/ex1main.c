#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;
	unsigned char cnt = 0x00;
while(1) {
		tmpA = PINA;
		tmpB = PINB;

		while (tmpA > 0)
		{
			if((tmpA & 0x01) == 1){
				cnt++;
			}
			tmpA = tmpA >> 1;
		}

		while (tmpB > 0)
		{
			if((tmpB & 0x01) == 1){
				cnt++;
			}
			tmpB = tmpB >> 1;
		}

		PORTC = cnt;	
	}
	return 0;
}

/*
test "PINA: 0x00 => PORTC: 0x00"
setPINA 0x00
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x03 => PORTC: 0x02"
setPINA 0x02
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x0F => PORTC: 0x84"
setPINA 0x01
continue 5
expectPORTB 0x84
checkResult

*/