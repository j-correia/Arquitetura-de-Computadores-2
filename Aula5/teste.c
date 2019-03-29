#include <detpic32.h>
void delay(int ms)
{
   for(; ms > 0; ms--)
   {
      resetCoreTimer();
      while(readCoreTimer() < 20000);
   }
}
unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + (value % 10);
}

int main(void)
{
	TRISBbits.TRISB0 = 0;
	LATE = LATE & 0xFFF0;
	TRISE = TRISE & 0xFFF0;
	// Configure A/D module and Port RB4 as Analog Input
	TRISBbits.TRISB4 = 1;	// RB4 digital output disconnect
	AD1PCFGbits.PCFG4 = 0; 	// RB configured as analog input
	AD1CON1bits.SSRC = 7;	// Conversion trigger select bits
	AD1CON1bits.CLRASAM = 1;// Stop conversion when 1 interrupt
				// is generated from ADC and clears
				// the ASAM bit
	AD1CON3bits.SAMC = 16;	// Sample time is 16 TAD 
				// ( TAD = 100ns)
	AD1CON2bits.SMPI = 0;	// 1 sample converted and BUFFERED
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC (must be last instr.)
	
	do
	{	
		// Start Conversion
		AD1CON1bits.ASAM = 1;
		// Wait while conversion not done (AD1IF == 0)
		while(IFS1bits.AD1IF == 0);
		// Read conversion result (ADC1BUF0 value)
		int V =(ADC1BUF0*33+511)/1023;
		V = toBcd(V)*10;
		int freq;
		int bit;
		if((V>= 0) && ( V < 102))
		{
			bit = 0;
			freq = 2;
		}
		else if( (V >= 102) && (V < 204))
		{
			bit = 0x1;
			freq = 4;
		}
		else if( (V >= 204) && (V < 306))
		{
			bit = 0x2;
			freq = 8;
		}
		else if( (V >= 306) && (V < 408))
		{
			bit = 0x4;
			freq = 16;
		}
		else if( (V >= 408) && (V <= 510))
		{
			bit = 0x8;
			freq = 32;
		}
		else{}
		// Reset AD1IF
		int time = 1000/freq;
		LATE = (LATE & 0xFFF0) + bit;
		printInt(V,0x0003000A);
		printStr(" ");
		printInt(freq,0x0002000A);
		printStr(" ");
		printInt(time, 0x0003000A);
		printStr("\r");
		
		IFS1bits.AD1IF = 0;
		delay(200);
	} while(1);
	return 0;
}

