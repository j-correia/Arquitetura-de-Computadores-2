#import <detpic32.h>

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}

int main(void)
{
	TRISBbits.TRISB4 = 1;		// Disconnect
	AD1PCFGbits.PCFG4 = 0;		// Analog input
	AD1CON1bits.SSRC = 7;		// internal counter
	AD1CON1bits.CLRASAM = 1;	// stop after 1 interrupt
	AD1CON3bits.SAMC = 16;		// 16 TAD (100ns per TAD)
	AD1CON2bits.SMPI = (1 - 1);	// Samples
	AD1CHSbits.CH0SA = 4;		// Analog input channel
	AD1CON1bits.ON = 1;		// Enable A/D
	
	while(1)
	{
		AD1CON1bits.ASAM = 1;		// Start Conversion
		while(IFS1bits.AD1IF == 0);
		printInt(ADC1BUF0, 16 | 3 << 16);
		printStr("\r");
		IFS1bits.AD1IF = 0;
		delay(20);	
	}
	return 0;
}
