#include <detpic32.h>

void delay(int ms)
{
	for(;ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}	
}

void configureAll(void)
{
	// Configure UART1
	// 1 - Configure BaudRate Generator
	U1MODEbits.BRGH = 0;
	U1BRG = ((20000000 + 8*115200)/(16*115200)) - 1;
	// 2 - Configure # of data bits, parity and # of stopbits
	//     ( see U1MODE register)
	// PDSEL<1:0>: Parity and Data Selection bits
	// 11 = 9-bit data, no parity
	// 10 = 8-bit data, odd parity
	// 01 = 8-bit data, even parity
	// 00 = 8-bit data, no parity
	U1MODEbits.PDSEL = 00;
	U1MODEbits.STSEL = 1;
	// 3 - Enable the Tx and Rx modules ( see register U1STA)
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// 4 - Enable UART1 ( see register U1MODE)
	U1MODEbits.ON = 1;
}

void putc(char byte2send)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte2send;
}

void puts(char *str)
{
	while(*str != '\0')
	{
		putc(*str);
		str++;
	}
}

int main(void)
{
	configureAll();
	while(1)
	{
		putc('+');
		puts("String the teste\n");
		delay(1000);
	}
	return 0;
}
