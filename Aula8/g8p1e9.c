#include <detpic32.h>

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);	
	}
}

void putc(char byte2send)
{
	// wait while(UTXBF == 1)
	while(U1STAbits.UTXBF);
	// Copy byte2send to the UxTXREG register
	U1TXREG = byte2send;
}

void puts(char *str)
{
	// use putc() to send each character ('\0') not sent
	while(*str != '\0')
	{
		putc(*str);
		str++;
	}
}

void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
	// DEFAULTS
	if( baud < 600 || baud > 115200)
	{
		baud = 115200;
	}
	if( parity!='N' && parity!='E' && parity!='O')
	{
		parity = 'N';
	}
	if( stopbits!=1 && stopbits!=2)
	{
		stopbits = 1;
	}
	// Configure UART1( 115200, N, 8, 1)
	// 1 - Configure BaudRate Generator
	U1BRG = ((20000000 + 8*baud)/(16*baud))-1;
	U1MODEbits.BRGH = 0;
	// 2 - Configure numer of data bits, parity and number 
	//     of stop bits (see U1MODE register)
	/*
	PDSEL<1:0>: Parity and Data Selection bits
	11 = 9-bit data, no parity
	10 = 8-bit data, odd parity
	01 = 8-bit data, even parity
	00 = 8-bit data, no parity
	*/
	if(parity == 'N')
	{
		U1MODEbits.PDSEL = 00;
	}
	else if(parity == 'E')
	{
		U1MODEbits.PDSEL = 01;
	}
	else if(parity == 'O')
	{
		U1MODEbits.PDSEL = 10;
	}
	/*
	STSEL: Stop Selection bit
	1 = 2 Stopbits
	0 = 1 Stopbit
{
{
	*/
	U1MODEbits.STSEL = ( --stopbits);
	// 3 - Enable the transmitter and receiver modules ( see
	//     register U1STA)
	U1STAbits.UTXEN = 1;		// Transmition Enable
	U1STAbits.URXEN = 1;		// Reception Enable
	// 4 - Enable UART1 ( see register U1MODE)
	U1MODEbits.ON = 1;
}

char getc(void)
{
	// OERR == 1 reset OERR
	if(U1STAbits.OERR)
		U1STAbits.OERR = 0;
	// wait while URXDA == 0
	while(!U1STAbits.URXDA);
	// If FERR or PERR then
	//	read UxRXREG (to discard character) and return 0
	if(U1STAbits.FERR || U1STAbits.PERR)
	{
		int discard = U1RXREG;
		return 0;
	}
	// else
	else
	{
		return U1RXREG;
	}
	//	return U1RXREG
}

int main(void)
{
	configUart(115200,'N',1);
	while(1)
	{
		putc(getc());
	}
	return 0;
}
