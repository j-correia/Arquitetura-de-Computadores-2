#include <detpic32.h>

int main(void)
{
	// Configure T3 with interrupts enable
	T3CONbits.TCKPS = 7;
	PR3=39062;
	TMR3=0;
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	// Configure T1 with interrupts enable
	T1CONbits.TCKPS = 7;
	PR1=39062;
	TMR1=0;
	T1CONbits.TON = 1;
	IPC1bits.T1IP = 2;
	IEC0bits.T1IE = 1;
	IFS0bits.T1IF = 0;
	EnableInterrupts();
	while(1);
	return 0;
}

void _int_(4) isr_T1(void)	// Timer 1 vector
{
	putChar('1');
	// Reset T1 interrupt flag
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)	// Timer 3 vector
{
	putChar('3');
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
}
