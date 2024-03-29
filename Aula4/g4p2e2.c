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
	int dh = value >> 4;
	dh = display7Scodes[dh];
	//printf("\n%d", dh);
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;
	dh = dh << 8;
	LATB = (LATB & 0x80FF) + dh;
	int dl = value & 0x0F;
	//printf("\n%d", dl);
	dl = display7Scodes[dl];
	//printf("\n%x", dl);
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	dl = dl << 8;
	printf("\n%x", dl);
	LATB = (LATB & 0x80FF) + dl;
}

int main(void)
{
	LATB = (LATB & 0x80FF);	
	TRISB = (TRISB & 0x80FF);
	//LATD = (LATD & 0xFF9F);
	//TRISD = (TRISD & 0xFF9F);
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	while(1)
	{
		unsigned char cont = 0;
		for( ; cont < 16; cont++)
		{
			send2displays(cont);
			delay(200);
		}
	}
	return 0;
}
