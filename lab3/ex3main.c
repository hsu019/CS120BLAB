#include <avr/io.h>
#ifdef _SIMULATE_
#include ¡°simAVRHeader.h¡±
#endif	

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpC = 0x00;
	unsigned char seatbelt = 0x00;
while(1) {
		tmpA = PINA;
		if ((tmpA & 0x70) == 0x30)
		{
			seatbelt = 0x01;
		}
		tmpA = tmpA & 0x0F;
		if ((tmpA == 0x01) || (tmpA == 0x02))
		{
			tmpC = 0x60;
		}
		else if ((tmpA == 0x03) || (tmpA == 0x04))
		{
			tmpC = 0x70;
		}
		else if ((tmpA == 0x05) || (tmpA == 0x06))
		{
			tmpC = 0x38;
		}
		else if ((tmpA == 0x07) || (tmpA == 0x08) || (tmpA == 0x09))
		{
			tmpC = 0x3C;
		}
		else if ((tmpA == 0x0A) || (tmpA == 0x0B)|| (tmpA == 0x0C))
		{
			tmpC = 0x3E;
		}
		else if ((tmpA == 0x0D) || (tmpA == 0x0E)|| (tmpA == 0x0F))
		{
			tmpC = 0x3F;
		}
		else {
			tmpC = 0x40;
		}
		

		if (seatbelt == 0x01)
		{
			tmpC = tmpC | 0x80;
		}

		PORTC = tmpC;	
	}
	return 0;
}

/*
test "PINA: 0x70 => PORTC: 0x40"
setPINA 0x70
continue 5
expectPORTC 0x40
checkResult

test "PINA: 0x53 => PORTC: 0x70"
setPINA 0x53
continue 5
expectPORTB 0x70
checkResult

test "PINA: 0xBB => PORTC: 0xBE"
setPINA 0xBB
continue 5
expectPORTB 0xBE
checkResult

test "PINA: 0x31 => PORTC: 0xE0"
setPINA 0x32
continue 5
expectPORTB 0xE0
checkResult

*/