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
	LATBbits.LATB6 = 0;
	TRISBbits.TRISB6 = 0;
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

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
	// 1 - Configure BaudRate Generator
	U1MODEbits.BRGH = 0;
	U1BRG = ((20000000 + 8*baud)/(16*baud)) - 1;
	// 2 - Configure # of data bits, parity and # of stopbits
	//     ( see U1MODE register)
	// PDSEL<1:0>: Parity and Data Selection bits
	// 11 = 9-bit data, no parity
	// 10 = 8-bit data, odd parity
	// 01 = 8-bit data, even parity
	// 00 = 8-bit data, no parity
	switch(parity)
	{
		case 'N':
			U1MODEbits.PDSEL = 00;
			break;
		case 'E':
			U1MODEbits.PDSEL = 01;
			break;
		case 'O':
			U1MODEbits.PDSEL = 10;
			break;
		default:
			break;
	}
	U1MODEbits.STSEL = stopbits - 1;
	// 3 - Enable the Tx and Rx modules ( see register U1STA)
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// 4 - Enable UART1 ( see register U1MODE)
	U1MODEbits.ON = 1;	
}

int main(void)
{
	configureAll();
	configUart(115200,'N',1);
	while(1)
	{
		while(U1STAbits.TRMT == 1);
		LATBbits.LATB6 = 1;
		puts("123456789AB");
		LATBbits.LATB6 = 0;
	}
	return 0;
}
