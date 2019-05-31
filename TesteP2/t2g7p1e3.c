#include<detpic32.h>
static int flag = 0;

int main(void)
{
	// Configure Timer T3 with interrupts enable
	EnableInterrupts();
	T3CONbits.TCKPS = 7;	// Presc = 256
	PR3 = 39062;
	TMR3 = 0;		// Reset timer T3 count register
	T3CONbits.TON = 1;	// Enable T3 timer
	IPC3bits.T3IP = 2;	// Interrupt Priority
	IEC0bits.T3IE = 1;	// Enable T3 interrupts
	IFS0bits.T3IF = 0;	// Reset Timer T3 IF
	while(1);
	return 0;
}

void _int_(12) isr_T3(void)
{
	if( !(flag % 2))
		putChar('.');
	// Reset T3 interrupt flag
	IFS0bits.T3IF = 0;
	flag++;
}
