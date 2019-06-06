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
	EnableInterrupts();
}

void putc(char byte2send)
{
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
	// Configure UART Interrupts
	// Configuração do TX e RX para
	U1STAbits.UTXSEL = 00;		// ativar posição livre
	// ESTA PARTE ESTÁ MAL IMPLEMENTADA E NECESSITA DE REVISÃO
	// O UTXSEL DEVIA SER UTXISEL
	U1STAbits.URXISEL = 00;		// ativar leitura nova
	// Configurar TX e RX para ativação/inativação
	IEC0bits.U1TXIE = 1;
	IEC0bits.U1RXIE = 1;
	// Configurar prioridade
	IPC6bits.U1IP = 3;
	// Configuar deteção de erros framming, overrun, parity
	IEC0bits.U1EIE = 1;
}

char getc(void)
{
	// If OERR == 1 then reset OERR
	if(U1STAbits.OERR == 1)
		U1STAbits.OERR == 0;
	// Wait while URXDA == 0
	//while(U1STAbits.URXDA == 0);
	// If FERR or PERR then
	// 	read UxRXREG (to discard the char) and return 0
	if(U1STAbits.FERR || U1STAbits.PERR)
	{
		int discard = U1RXREG;
		return 0;
	}
	// Else
	//	return U1RXREG
	else
		return U1RXREG;
}

int main(void)
{
	configureAll();
	configUart(115200,'N',1);
	while(1)
	{
		//putc(getc());
	}
	return 0;
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1RXIF)
	{
		putc(U1RXREG);
	}
	// Clear UART1 RX interrupt flag
	if(IFS0bits.U1TXIF)
	{
	}
	IFS0bits.U1RXIF = 0;
	IFS0bits.U1TXIF = 0;
}
