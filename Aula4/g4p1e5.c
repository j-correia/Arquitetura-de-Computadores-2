#include <detpic32.h>

void delay(int ms);
int main(void)
{
	unsigned char segment;
	//LATDbits.LATD6 = 1;
	//LATDbits.LATD5 = 0;
	TRISB = TRISB & 0x00FF;
	TRISDbits.TRISD6 = 0;
	TRISDbits.TRISD5 = 0;
	LATD = (LATD & 0xFF9F) | 0x0040;
	/*LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	LATB = LATB & 0x00FF;
	TRISB = (TRISB & 0x00FF);*/
	int i;
	while(1)
	{
		LATD = LATD ^ 0x0060;
		segment = 1;
		for(i = 0; i < 8; i++)
		{
			// send segment to display
			int bit = segment << 8;
			LATB = (LATB & 0x00FF) + bit;
			delay(10);
			segment = segment << 1; 
		}
	}
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}
}
