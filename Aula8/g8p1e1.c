#include <detpic32.h>

int main(void)
{
	// Configure UART1:
	
	// 1 - Configure BaudRate Generator
	U1BRG = (20000000 / (16*115200))-1;
	U1MODEbits.BRGH = 1;
	// 2 - Configure numer of data bits, parity and number 
	//     of stop bits (see U1MODE register)
	/*
	PDSEL<1:0>: Parity and Data Selection bits
	11 = 9-bit data, no parity
	10 = 8-bit data, odd parity
	01 = 8-bit data, even parity
	00 = 8-bit data, no parity
	*/
	U1MODEbits.PDSEL = 00;
	/*
	STSEL: Stop Selection bit
	1 = 2 Stopbits
	0 = 1 Stopbit
	*/
	U1MODEbits.STSEL = 0;
	// 3 - Enable the transmitter and receiver modules ( see
	//     register U1STA)
	U1STAbits.UTXEN = 1;		// Transmition Enable
	U1STAbits.URXEN = 1;		// Reception Enable
	// 4 - Enable UART1 ( see register U1MODE)
	U1MODEbits.ON = 1;
	
	return 0;
} 
