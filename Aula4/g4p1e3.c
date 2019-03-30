#include <detpic32.h>

void delay(int ms);
int main(void)
{
	int c;
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	LATB = LATB & 0x00FF;
	TRISB = (TRISB & 0x00FF);
	int displaySeg[] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40};
			//  A  , B  , C  , D  , E   , F   , G
	while(1)
	{
		c = inkey();	
		if(c >= 'A' && c <= 'G')
		{
			printInt(c,0x0002000F);
			int dsp = c - 65;
			printInt(dsp,0x0002000A);
			printStr("\r");
			int seg = displaySeg[dsp] << 8;
			LATB =(LATB & 0x00FF) + seg;	
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
