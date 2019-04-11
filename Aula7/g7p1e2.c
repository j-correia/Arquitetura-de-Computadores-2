#include <detpic32.h>

int main(void)
{
	// Configure T3 with interrupts enable
	int kpr = 256;
	int freq = 2;
	T3CONbits.TCKPS = 7;
	PR3=PBCLK/kpr/freq;
	TMR3=0;
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	EnableInterrupts();
	while(1);
	return 0;
}

void _int_(12) isr_T3(void)	// Timer 3 vector
{
	putChar('.');
	// Reset T3 interrupt flag
}
