#include <detpic32.h>

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}
}

void main(void)
{
	static const int display7Scodes[] =  {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
	TRISB = (TRISB & 0xFFF0) | 0x000F;
	LATB = (LATB & 0x80FF);
	TRISB = (TRISB & 0x80FF);
	LATDbits.LATD6 = 0;
	LATDbits.LATD5 = 0;
	TRISDbits.TRISD6 = 0;
	TRISDbits.TRISD5 = 0;
	LATDbits.LATD5 = 1;
	while(1)
	{
		// read dip-switch
		int dipswitch = (PORTB & 0x000F) & 0xF;
		// convert to 7 segment code
		int disp7seg = display7Scodes[dipswitch];
		//printf("\n%d", dipswitch);
		// send to display
		disp7seg = disp7seg << 8;
		LATB = (LATB & 0x00FF) + disp7seg;
		delay(1);
	}	
}
