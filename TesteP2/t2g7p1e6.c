#include <detpic32.h>
static int v;

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
	static char displayFlag = 0;
	int dh = value >> 4;
	int dl = value & 0x0F;
	if(displayFlag == 0)
	{
		dl = display7Scodes[dl];
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		dl = dl << 8;
		LATB = (LATB & 0x80FF) + dl;
	}
	else
	{
		dh = display7Scodes[dh];
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		dh = dh << 8;
		LATB = (LATB & 0x80FF) + dh;
	}
	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + (value % 10);
}

void configureAll(void)
{
	EnableInterrupts();
	// Configure Timer T1 with interrupts enable
	T1CONbits.TCKPS = 3;	// Presc = 256
	PR1 = 19531;
	TMR1 = 0;		// Reset timer T1 count register
	T1CONbits.TON = 1;	// Enable T1 timer
	IPC1bits.T1IP = 2;	// Interrupt Priority
	IEC0bits.T1IE = 1;	// Enable T1 interrupts
	IFS0bits.T1IF = 0;	// Reset Timer T1 IF
	// Configure Timer T3 with interrupts enable
	T3CONbits.TCKPS = 2;	// Presc = 4
	PR3 = 49999;
	TMR3 = 0;		// Reset timer T3 count register
	T3CONbits.TON = 1;	// Enable T3 timer
	IPC3bits.T3IP = 2;	// Interrupt Priority
	IEC0bits.T3IE = 1;	// Enable T3 interrupts
	IFS0bits.T3IF = 0;	// Reset Timer T3 IF
	
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
	AD1CON2bits.SMPI = 7;	// 8-1 samples converted and BUFFERED
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC (must be last instr.)
	// Configure interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
	// Misc Ports
	TRISB = (TRISB & 0xFFFC) | 0x0003;
}

int main(void)
{
	configureAll();
	while(1)
	{
		//if((PORTB & 0x0002) == 0x0001)
		if((PORTBbits.RB1 == 0) && (PORTBbits.RB0 == 1))
			IEC0bits.T1IE = 0;
		else
			IEC0bits.T1IE = 1;
	}
	return 0;
}

void _int_(4) isr_T1(void)
{
	v = 0;
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	send2displays(v);
	IFS0bits.T3IF = 0;
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
	for(; p <= (int *)(&ADC1BUF7); p+=4)
	{
		//printInt(*p, 10 | 4 << 16);
		//printStr(" "); 
		v += *p;
	}
	v= v/8;
	v = (v * 33 + 511)/1023;
	v = toBcd(v);
	// Reset AD1IF
	IFS1bits.AD1IF = 0;
}
