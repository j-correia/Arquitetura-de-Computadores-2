#import <detpic32.h>
void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}
void send2displays(unsigned char value)
{
	static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	static char displayFlag = 0; // static não perde valor
	// send dh: dh >> 4
	int dl = value & 0x0F;
	dl = display7Scodes[dl];
	dl = dl << 8;
	// send dl: dl = value & 0x0F
	int dh = value >> 4;
	dh = display7Scodes[dh];
	dh = dh << 8;
	if(displayFlag == 0)
	{
		LATD = (LATD & 0xFF9F) + 0x0020;
		LATB = (LATB & 0x80FF) + dl;
	}
	else
	{
		LATD = (LATD & 0xFF9F) + 0x0040;
		LATB = (LATB & 0x80FF) + dh;
	}
	displayFlag = !displayFlag;
}
int main(void)
{
	LATB = LATB & 0x80FF;
	TRISB = (TRISB & 0x80FF);
	LATD = (LATD & 0xFF9F);
	TRISD = TRISD & 0xFF9F;
	while(1)
	{
		int value = 99;
		send2displays(value);
		delay(2);
	}
	return 0;
}
