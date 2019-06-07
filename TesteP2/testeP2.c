// ler switch 3 e 4 e passar para led 0 e 1
#include <detpic32.h>

int  cont = 0;
int v;

void delay(int ms)
{
	resetCoreTimer();
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
	
}

void configAll(void)
{
	EnableInterrupts();
	LATB = (LATB & 0x80FF);
	TRISB = (TRISB & 0x80FF);
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 0;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	TRISB = (TRISB & 0xFFF3) | 0x000C;
	LATE = (LATE & 0xFFFC);
	TRISE = (LATE & 0xFFFC);
	T3CONbits.TCKPS = 1;
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;	// Enable T3
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;	// Enable T3 interrups
	IFS0bits.T3IF = 0;	// reset t3 flag
	
	// t2
	T2CONbits.TCKPS = 6;
	PR2 = 62499;
	TMR2 = 0;
	T2CONbits.TON = 1;
	IPC2bits.T2IP = 2;
	IEC0bits.T2IE = 1;
	IFS0bits.T2IF = 0;
	
	// OCM
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 0;
	OC1CONbits.ON = 1;
	
	// analog
	TRISBbits.TRISB4 = 1;	// RB4 digital output disconnect
	AD1PCFGbits.PCFG4 = 0; 	// RB configured as analog input
	AD1CON1bits.SSRC = 7;	// Conversion trigger select bits
	AD1CON1bits.CLRASAM = 1;// Stop conversion when 1 
	AD1CON3bits.SAMC = 16;	// Sample time is 16 TAD 
	AD1CON2bits.SMPI = 3;	// 4-1 samples converted
	AD1CHSbits.CH0SA = 4;	// Select AN4 as input for ADC
	AD1CON1bits.ON = 1;	// Enable ADC 
	// Configure interrupt system
	IPC6bits.AD1IP = 2;	// Configure priority of A/D
	IEC1bits.AD1IE = 1;	// enable A/D Interrupts
	IFS1bits.AD1IF = 0;	// Reset AD1IF flag
	EnableInterrupts();	// Global Interrupt Enable
}

unsigned char toBcd(unsigned char value)
{
	return(( value / 10) << 4) + (value % 10);
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

int main(void)
{
	configAll();
	int time = 0;
	int verTime = 0;
	while(1)
	{
		cont++;
		if(cont == 100)
			cont = 0;
		printInt10(cont);
		delay(200);
	}
	return 0;
}

void _int_(27) isr_ADC(void)
{
	v = 0;
	int *p = (int *)(&ADC1BUF0);
	for(; p <= (int *)(&ADC1BUF3); p+=4)
	{
		v = v + *p;
	}
	v = v/4;
	v = (v * 99 + 511)/1023;
	v = toBcd(v);
	OC1RS = ((PR3 + 1) * v)/ 100;
	IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3(void)
{
	LATE = (LATE & 0xFFFC) | ((PORTB & 0x000C) >> 2);
	send2displays(v);	
	AD1CON1bits.ASAM = 1;
	IFS0bits.T3IF = 0;
}

void _int_(8) isr_T2(void)
{
	IFS0bits.T2IF = 0;
}
