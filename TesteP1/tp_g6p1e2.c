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

volatile unsigned int voltage = 0; // global

int main(void)
{
	unsigned int cnt = 0;
	// Config all digital I/O, analog input, A/D module
	LATB = LATB & 0x80FF;
	LATD = LATD & 0xFF9F;
	TRISB = (TRISB & 0x80EF) | 0x0010;// RB4 disconnect
	TRISD = TRISD & 0xFF9F;
	AD1PCFGbits.PCFG4 = 0;	// AN4 input
	AD1CON1bits.SSRC = 7;	// internal counter
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;	// 16TAD
	AD1CON2bits.SMPI = (8 - 1); // samples
	AD1CHSbits.CH0SA = 4;	// Analog input channel
	AD1CON1bits.ON = 1;	// Enable AD
	// Config interrupt system
	IPC6bits.AD1IP = 2;	// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;	// clear A/D flag
	IEC1bits.AD1IE = 1;	// enable A/D
	EnableInterrupts();
	while(1)
	{
		// 250ms (4 samples/second)
		if(cnt % 25 == 0)
		{
			// Start A/D conversion
			AD1CON1bits.ASAM = 1;	// Start Conversion
		}
		send2displays(toBcd(voltage));
		// print ADC1BUF0
		printStr("\r");
		printInt(voltage, 10 | 3 << 16);
		cnt++;
		delay(10);
	}
	return 0;
}
// rsi
void _int_(27) isr_adc(void)
{
	voltage = 0;
	int  *p = (int *)(&ADC1BUF0);	
	for(; p <= (int *)&ADC1BUF7; p+=4) // (8-1) amostras
	{
		voltage += *p;
	}
	voltage = voltage / 8;
	voltage = (voltage * 33 + 511)/1023;
	// Start A/D onversion
	//AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
