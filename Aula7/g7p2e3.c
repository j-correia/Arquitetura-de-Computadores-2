#include <detpic32.h>

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

int main(void)
{
	TRISEbits.TRISE0 = 0;
	LATEbits.LATE0 = 0;
	TRISDbits.TRISD0 = 0;
	LATDbits.LATD0 = 0;
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
	while(1);
	return 0;
}

void _int_(12) isr_T3(void)
{
	LATDbits.LATD0 = !LATDbits.LATD0;
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
}
