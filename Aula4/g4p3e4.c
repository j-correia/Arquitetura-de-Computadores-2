#include <detpic32.h>

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
	static char displayFlag = 0;
	int dh = value >> 4;
	int dl = value & 0x0F;
	if(displayFlag == 0)
	{
		dl = display7Scodes[dl];
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		dl = dl << 8;
		LATB = (LATB & 0x80FF) + dl;
	}
	else
	{
		dh = display7Scodes[dh];
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		dh = dh << 8;
		LATB = (LATB & 0x80FF) + dh;
	}
	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + (value % 10);
}

int main(void)
{
	LATB = (LATB & 0x80FF);	
	TRISB = (TRISB & 0x80FF);
	LATD = (LATD & 0xFF9F);
	TRISD = (TRISD & 0xFF9F);
	int counter = 0;
	while(1)
	{
		int i = 0;
		do
		{
			delay(10);
			send2displays(toBcd(counter));
		}
		while( ++i < 100);
		counter++;
		if(counter > 99)
		{
			counter = 0;
			int j = 0;
			do
			{
				delay(500);
				send2displays(00);
			}
			while(++j < 10);
		}
	}
	return 0;
}
