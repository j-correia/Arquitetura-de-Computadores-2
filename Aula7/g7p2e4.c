#include <detpic32.h>
int voltage = 0;
void setPWM1(unsigned int dutyCycle)
{
	OC1RS = ((PR1+1)*dutyCycle)/100;
}

void setPWM2(unsigned int dutyCycle)
{
	OC1RS = ((PR2+1)*dutyCycle)/100;
}

void setPWM3(unsigned int dutyCycle)
{
	OC1RS = ((PR3+1)*dutyCycle)/100;
}

// Interrupt Handler
void _int_(27) isr_adc(void)	// Replace Vector by ADC
{					// vector number
	// Start ADC
	//AD1CON1bits.ASAM = 1;
	// Print ADC1BUF0 value in 0x and 3 digits
	// Read conversion result (ADC1BUF0 value)
	// print it
	int *p = (int *)(&ADC1BUF0);
	//printStr("\r");
	for(; p <= (int *)(&ADC1BUF3); p+=4)
	{
		//printInt(*p, 10 | 4 << 16);
		//printStr(" "); 
		voltage += *p;
	}
	voltage= voltage/4;
	voltage = (voltage * 33 + 511)/1023;
	voltage = toBcd(v);
	// Reset AD1IF
	IFS1bits.AD1IF = 0;
}

void configureAll(void)
{
	// Configure all (digital I/O analog input, A/D module)
	LATB = LATB & 0x80FF;
	TRISB = TRISB & 0x80FF;
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	// Configure A/D module and Port RB4 as Analog Input
	TRISBbits.TRISB4 = 1;	// RB4 digital output disconnect
	AD1PCFGbits.PCFG4 = 0; 	// RB configured as analog input
	AD1CON1bits.SSRC = 7;	// Conversion trigger select bits
	AD1CON1bits.CLRASAM = 1;// Stop conversion when 1 interrupt
				// is generated from ADC and clears
				// the ASAM bit
	AD1CON3bits.SAMC = 16;	// Sample time is 16 TAD 
				// ( TAD = 100ns)
	AD1CON2bits.SMPI = 3;	// 4-1 samples converted and BUFFERED
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC (must be last instr.)
	// Configure A/D interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
}

int main(void)
{
	int dutyCycle;
	configureAll();
	TRISBbits.TRISB1 = 1;
	TRISBbits.TRISB0 = 1;
	T3CONbits.TCKPS = 2; // 1:256 prescaler (i.e fin = 78,125 KHz)
	PR3 = 49999; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;	
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;
	setPWM3(25);
	EnableInterrupts();
	while(1)
	{
		unsigned int portVal = (PORTBbits.RB1 + PORTBbits.RB0) && 0x0003;
		// Read RB1..0 to the variable portVal
		switch(portVal)
		{
			case 0:	// Measure input voltage
				// Enable T1 interrupts
				setPWM1(0);
				break;
			case 1:	// Freeze
				// Disable T1 interrupts
				setPWM1(100);
				break;
			default:// LED brigthness control
				// Enable T1 interrupts
				dutyCycle = voltage * 3;
				setPWM1(dutyCycle);
				break;				
		}
		return 0;
	}
}

void _int_(4) isr_T1(void)
{
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
}
