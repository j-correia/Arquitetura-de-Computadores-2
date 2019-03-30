#include <detpic32.h>

int main(void)
{
	// Congigure Timer T3 (2 Hz with interrupts disable)
	T3CONbits.TCKPS = 7; 	// 1:256 prescaler
	PR3 = 39062;		// Fout = 2Hz
	TMR3 = 0;		// Reset timer register
	T3CONbits.TON = 1;
	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	while(1)
	{
		while(IFS0bits.T3IF == 0);
		// Reset T3IF
		IFS0bits.T3IF = 0;
		putChar('.');
	}
	return 0;
}
