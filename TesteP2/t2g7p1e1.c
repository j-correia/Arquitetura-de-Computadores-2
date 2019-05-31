#include<detpic32.h>

int main(void)
{
	// Configure timer T3 (2 Hz with interrupts disable)
	T3CONbits.TCKPS = 7;	// Presc = 256
	PR3 = 39062;
	TMR3 = 0;		// Reset timer T3 count register
	T3CONbits.TON = 1;	// Enable T3 timer
	while(1)
	{
		// wait until T3IF = 1
		while(!IFS0bits.T3IF);
		// Reset T3IF
		IFS0bits.T3IF = 0;
		putChar('.');
	}
	return 0;
}
