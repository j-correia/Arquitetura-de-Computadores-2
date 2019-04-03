#import <detpic32.h>
void delay(int ms);
int main(void)
{
	unsigned char segment;
	int bit;
	int i;
	LATDbits.LATD6 = 1;
	LATDbits.LATD5 = 0;
	LATB = LATB & 0x80FF;
	TRISB = TRISB & 0x80FF;		// RB8..14 as output
	TRISD = TRISD & 0xFF9F;		// RD5..6 as output
	while(1)
	{
		LATDbits.LATD6 = !LATDbits.LATD6;
		LATDbits.LATD5 = !LATDbits.LATD5;
		segment = 1;
		for(i = 0; i < 7; i++)
		{
			// send segment to display
			bit = segment << 8;
			LATB = (LATB & 0x00FF) + bit;
			delay(500);
			segment = segment << 1; 
		}
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
