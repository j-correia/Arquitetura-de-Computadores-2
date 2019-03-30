// Tempo medido: 4.32us	
#include <detpic32.h>

volatile int adc_value;

int main(void)
{
	// Configure all (digital I/O analog input, A/D module)
	LATEbits.LATE0 = 0;
	TRISEbits.TRISE0 = 0;
	// Configure A/D module and Port RB4 as Analog Input
	TRISBbits.TRISB4 = 1;	// RB4 digital output disconnect
	AD1PCFGbits.PCFG4 = 0; 	// RB configured as analog input
	AD1CON1bits.SSRC = 7;	// Conversion trigger select bits
	AD1CON1bits.CLRASAM = 1;// Stop conversion when 1 interrupt
				// is generated from ADC and clears
				// the ASAM bit
	AD1CON3bits.SAMC = 16;	// Sample time is 16 TAD 
				// ( TAD = 100ns)
	AD1CON2bits.SMPI = 0;	// 1-1 samples converted and BUFFERED
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC (must be last instr.)
	// Configure interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
	EnableInterrupts();	// Global Interrupt Enable
	// Start A/D conversion
	AD1CON1bits.ASAM = 1;
	while(1)
	{
		// All activity is done by the ISR
	}
	return 0;
}

// Interrupt Handler
void _int_(27) isr_adc(void)	// Replace Vector by ADC
{					// vector number
	LATEbits.LATE0 = 0;
	adc_value = ADC1BUF0;
	LATEbits.LATE0 = 1;
	// Start ADC
	AD1CON1bits.ASAM = 1;
	// Reset AD1IF
	IFS1bits.AD1IF = 0;
}
