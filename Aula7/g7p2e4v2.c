#include <detpic32.h>
volatile unsigned int v;

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
// Configure all (digital I/O analog input, A/D module)
	LATB = LATB & 0x80FF;
	TRISB = (TRISB & 0x80FF) | 0x0003;
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
	// Configure interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
	// Configure T3 with interrupts enable
	T3CONbits.TCKPS = 2;
	PR3=49999;
	TMR3=0;
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 4;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	// Configure T1 with interrupts enable
	T1CONbits.TCKPS = 3;
	PR1=19530;
	TMR1=0;
	T1CONbits.TON = 1;
	IPC1bits.T1IP = 4;
	IEC0bits.T1IE = 1;
	IFS0bits.T1IF = 0;
	EnableInterrupts();	// Global Interrupt Enable
}

void setPWM(unsigned int dutyCycle)
{
	// duty_cycle must be in range [0, 100]
	if(dutyCycle < 0 || dutyCycle > 100)
		dutyCycle = 50;			// default
	OC1RS = ((PR1+1)*dutyCycle)/100;
	
}

int main(void)
{
	configureAll();
	EnableInterrupts();
	int dutyCycle;

	TRISEbits.TRISE4 = 1;
	TRISEbits.TRISE5 = 1;
	TRISEbits.TRISE6 = 1;
	TRISEbits.TRISE7 = 1;
	T3CONbits.TCKPS = 2; // 1:256 prescaler (i.e fin = 78,125 KHz)

	/* configuração da luz */
	PR3 = 49999; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;

	while(1)
	{
		// Read RB1, RB0 to portVal
		unsigned int portVal = (PORTBbits.RB1 << 1) + (PORTBbits.RB0);
		//printStr("\r");
		//printInt(portVal,10);
		switch(portVal)
		{
			case 0:	// Measure input voltage
				IEC1bits.AD1IE = 1;
				// Enable T1 interrupts
				IEC0bits.T1IE = 1;
				setPWM(0);	// Led OFF
				
				break;
			case 1:	// Freeze
				IEC1bits.AD1IE = 0;
				// Disable T1 interrupts
				IEC0bits.T1IE = 0;
				setPWM(100);	// LED ON
			
				break;
			default:// LED brigthness control
				IEC1bits.AD1IE = 1;
				// Enable T1 interrupts
				IEC0bits.T1IE = 1;
//				int val  = ((v & 0xF0) >> 4) + (v & 0x0F);
				dutyCycle = v * 3;
				//printStr("\r");
				//printf(v + " " + dutyCycle);
				setPWM(dutyCycle);
				break;
		}
	}
	return 0;
}


// Interrupt Handler
void _int_(27) isr_adc(void)	// Replace Vector by ADC
{					// vector number
	// Start ADC
	//AD1CON1bits.ASAM = 1;
	// Print ADC1BUF0 value in 0x and 3 digits
	// Read conversion result (ADC1BUF0 value)
	// print it
	v = 0;
	int *p = (int *)(&ADC1BUF0);
	//printStr("\r");
	for(; p <= (int *)(&ADC1BUF3); p+=4)
	{
		//printInt(*p, 10 | 4 << 16);
		//printStr(" "); 
		v += *p;
	}
	v = v / 4;
	v = (v * 33 + 511)/1023;
	//printInt(v,10);
//	v = toBcd(v);
	// Reset AD1IF
	IFS1bits.AD1IF = 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1; /* start conversion */
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	send2displays(toBcd(v));
	IFS0bits.T3IF = 0;

}
