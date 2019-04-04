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
	AD1CON2bits.SMPI = (16 - 1);	// Samples
	AD1CHSbits.CH0SA = 4;		// Analog input channel
	AD1CON1bits.ON = 1;		// Enable A/D
	
	while(1)
	{
		AD1CON1bits.ASAM = 1;		// Start Conversion
		while(IFS1bits.AD1IF == 0);
		int *p = (int *)(&ADC1BUF0);
                printStr("\r");
                for(; p <= (int *)(&ADC1BUFF	); p+=4)
                {
		        printInt(*p, 10 | 4 << 16);
		        printStr(" "); 
                }
		IFS1bits.AD1IF = 0;
		delay(20);	
	}
	return 0;
}
