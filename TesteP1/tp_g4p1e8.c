#import <detpic32.h>
void delay(int ms);

int main(void)
{
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	TRISB = (TRISB & 0xFFF0) | 0x000F;
	LATB = LATB & 0x80FF;
	TRISB = (TRISB & 0x80FF);
	LATD = (LATD & 0xFF9F) | 0x0020;
	TRISD = TRISD & 0xFF9F;
	while(1)
	{
		int value = PORTB & 0x000F;
		value = display7Scodes[value];
		value = value << 8;
		LATB = (LATB & 0x80FF) + value;
		delay(1);
	}
	return 0;
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}
