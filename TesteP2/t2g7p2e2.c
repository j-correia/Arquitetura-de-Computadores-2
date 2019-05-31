#include <detpic32.h>

void configureAll()
{
	EnableInterrupts();
	// Configure Timer 3 with 25% Duty-Cycle
	T3CONbits.TCKPS = 2; 
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	OC1CONbits.OCM = 6;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.ON = 1;
}

void setPWM3(unsigned int dutyCycle)
{
	// Duty_cycle in range [0,100]
	OC1RS = (PR3*dutyCycle)/100;
}

int main(void)
{
	configureAll();
	setPWM3(1);
	while(1);
	return 0;
}

void _int_(12) isr_T3(void)
{
	LATDbits.LATD0 = !LATDbits.LATD0;
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;	
}
