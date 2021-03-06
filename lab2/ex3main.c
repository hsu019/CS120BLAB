#include <avr/io.h>
#ifdef _SIMULATE_
#include ��simAVRHeader.h��
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char cntavail = 0x00;
while(1) {
		tmpA = PINA;
		if ((tmpA & 0x01) == 0x01) { 
			cntavail = cntavail + 1;
		}
		if ((tmpA & 0x02) == 0x02) { 
			cntavail = cntavail + 1;
		}
		if ((tmpA & 0x04) == 0x04) { 
			cntavail = cntavail + 1;
		}
		if ((tmpA & 0x08) == 0x08) { 
			cntavail = cntavail + 1;
		}	
		if (cntavail == 0x04)
		{
			cntavail = (cntavail & 0x7F) | 0x80;
		}
		PORTC = cntavail;	
	}
	return 0;
}

/*
test "PINA: 0x00 => PORTC: 0x04"
setPINA 0x00
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x0B => PORTC: 0x01"
setPINA 0x02
continue 5
expectPORTB 0x00
checkResult

test "PINA: 0x0F => PORTC: 0x80"
setPINA 0x01
continue 5
expectPORTB 0x84
checkResult

*/