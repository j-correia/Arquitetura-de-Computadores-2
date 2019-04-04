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

unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + ( value % 10);
}

int main(void)
{
	LATB = LATB & 0x80FF;
	LATD = LATD & 0xFF9F;
	TRISB = TRISB & 0x80FF;
	TRISD = TRISD & 0xFF9F;
	TRISBbits.TRISB4 = 1;		// Disconnect
	AD1PCFGbits.PCFG4 = 0;		// Analog input
	AD1CON1bits.SSRC = 7;		// internal counter
	AD1CON1bits.CLRASAM = 1;	// stop after 1 interrupt
	AD1CON3bits.SAMC = 16;		// 16 TAD (100ns per TAD)
	AD1CON2bits.SMPI = (4 - 1);	// Samples
	AD1CHSbits.CH0SA = 4;		// Analog input channel
	AD1CON1bits.ON = 1;		// Enable A/D
	int i = 0;
	while(1)
	{
		int V;
		if(i++ % 25 == 0)
		{
			V = 0;
			AD1CON1bits.ASAM = 1;	// Start Conversion
			while(IFS1bits.AD1IF == 0);
			int *p = (int *)(&ADC1BUF0);
                	for(; p <= (int *)(&ADC1BUF3); p+=4)
                	{
			        V += *p;
                	}
                	IFS1bits.AD1IF = 0;
                	V /= 4;
                	V = (V * 33 + 511)/1023;
		}
		send2displays(toBcd(V));
		delay(10);
	}
	return 0;
}
