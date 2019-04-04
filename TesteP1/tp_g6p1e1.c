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
	// Config all digital I/O, analog input, A/D module
	TRISB = (TRISB & 0xFFEF) | 0x0010;// RB4 disconnect
	AD1PCFGbits.PCFG4 = 0;	// AN4 input
	AD1CON1bits.SSRC = 7;	// internal counter
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;	// 16TAD
	AD1CON2bits.SMPI = (1 - 1); // samples
	AD1CHSbits.CH0SA = 4;	// Analog input channel
	AD1CON1bits.ON = 1;	// Enable AD
	// Config interrupt system
	IPC6bits.AD1IP = 2;	// configure priority of A/D interrupts
	IFS1bits.AD1IF = 0;	// clear A/D flag
	IEC1bits.AD1IE = 1;	// enable A/D
	EnableInterrupts();
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;	// Start Conversion
	while(1)
	{
		//
	}
	return 0;
}
// rsi
void _int_(27) isr_adc(void)
{
	// print ADC1BUF0
	printStr("\r");
	printInt(ADC1BUF0, 16 | 3 << 16);
	// Start A/D onversion
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}
