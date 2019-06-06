#include <detpic32.h>
#define BUF_SIZE 			32
#define INDEX_MASK			BUF_SIZE - 1
#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE	= 1

typedef struct
{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
}circularBuffer;

volatile circularBuffer rxb;
volatile circularBuffer txb;

void comDrv_flushRx(void)
{
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
	int i = 0;
	for(; i < BUF_SIZE; i++)
		rxb.data[i] = 0;
}

void comDrv_flushTx(void)
{
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
	int i = 0;
	for(; i < BUF_SIZE; i++)
		txb.data[i] = 0;
}

void comDrv_putc(char ch)
{
	while(txb.count == BUF_SIZE);
	txb.data[txb.tail] = ch;
	txb.tail = (txb.tail + 1) & INDEX_MASK;
	DisableUart1TxInterrupt();
	txb.count++;
	EnableUart1TxInterrupt();
}

void comDrv_puts(char *s)
{
	while(*s != '\0')
	{
		comDrv_putc(*s);
		s++;		
	}
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1TXIF == 1)
	{
		if(txb.count > 0)
		{
			U1TXREG = txb.data[txb.head];
			txb.head = (txb.head + 1) & INDEX_MASK;
			txb.count--; 
		}
		if(txb.count == 0)
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;
	}
	
	if(IFS0bits.U1RXIF == 1)
	{
		rxb.data[rxb.tail] = U1RXREG;
		rxb.tail = (rxb.tail + 1) & INDEX_MASK;
		if(rxb.count < BUF_SIZE)
			rxb.count++;
		else
			rxb.head++;
		IFS0bits.U1RXIF = 0;
	}
}

char comDrv_getc(char *pchar)
{
	if(rxb.count == 0)
		return 0;
	DisableUart1RxInterrupt();
	*pchar = rxb.data[rxb.head];
	rxb.count--;
	rxb.head = (rxb.head + 1) & INDEX_MASK;
	EnableUart1RxInterrupt();
	return 1;
}
void comDrv_config(unsigned int baud, char parity, unsigned int stopbits)
{
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
		U1MODEbits.PDSEL = 0;
	}
	else if(parity == 'E')
	{
		U1MODEbits.PDSEL = 1;
	}
	else if(parity == 'O')
	{
		U1MODEbits.PDSEL = 0b10;
	}
	/*
	STSEL: Stop Selection bit
	1 = 2 Stopbits
	0 = 1 Stopbit
	*/
	U1MODEbits.STSEL = ( --stopbits);
	// 3 - Ativar módulos de transmissão e recepção
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;
	// 4 - Ativar UART
	U1MODEbits.ON = 1;
	/*
	
	*/
	// Configure UART Interrupts
	// Enable Interruption Sources
	EnableUart1TxInterrupt();
	EnableUart1RxInterrupt();
	// Configurar prioridade
	IPC6bits.U1IP = 3;
	// Configuração do TX e RX para
	U1STAbits.UTXSEL = 00;		// ativar posição livre
	// Nome do bits NECESSITA DE REVISÃO
	// O UTXSEL DEVIA SER UTXISEL
	U1STAbits.URXISEL = 00;	
}

int main(void)
{
	EnableInterrupts();
	comDrv_flushTx();
	comDrv_flushRx();
	comDrv_config(115200,'N',1);
	char a;
	comDrv_puts("Da graça de Joao Correia.");
	while(1)
	{
		if(comDrv_getc(&a))
		{
			if(a < 0x7B && a > 0x60)
			{
				a = a - 0x20;
				comDrv_putc(a);
			}
			else if(a == 10)
				comDrv_puts("Joao Correia");
			else
				comDrv_putc(a);			
		}
	}
	return 0;
}
