#include<detpic32.h>

void configureAll(void)
{
	// Configure Timer T1 with interrupts enable
	EnableInterrupts();
	T1CONbits.TCKPS = 3;	// Presc = 256
	PR1 = 39062;
	TMR1 = 0;		// Reset timer T1 count register
	T1CONbits.TON = 1;	// Enable T1 timer
	IPC1bits.T1IP = 2;	// Interrupt Priority
	IEC0bits.T1IE = 1;	// Enable T1 interrupts
	IFS0bits.T1IF = 0;	// Reset Timer T1 IF
	// Configure Timer T3 with interrupts enable
	EnableInterrupts();
	T3CONbits.TCKPS = 5;	// Presc = 256
	PR3 = 62499;
	TMR3 = 0;		// Reset timer T3 count register
	T3CONbits.TON = 1;	// Enable T3 timer
	IPC3bits.T3IP = 2;	// Interrupt Priority
	IEC0bits.T3IE = 1;	// Enable T3 interrupts
	IFS0bits.T3IF = 0;	// Reset Timer T3 IF
}

int main(void)
{
	configureAll();
	while(1);
	return 0;
}

void _int_(4) isr_T1(void)
{

}

void _int_(12) isr_T3(void)
{

}
