#include <detpic32.h>
int flag = 0;
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
	EnableInterrupts();
	while(1);
	return 0;
}

void _int_(12) isr_T3(void)	// Timer 3 vector
{
	if(flag == 1)
	{
		putChar('.');
	}
	flag = !flag;
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
}
