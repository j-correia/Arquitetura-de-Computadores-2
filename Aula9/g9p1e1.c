#include <detpic32.h>
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()  IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt()  IEC0bits.U1TXIE = 1
#define BUF_SIZE 		  8
#define INDEX_MASK 		  (BUF_SIZE - 1)

typedef struct
{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
	unsigned int overrun;
}circularBuffer;

volatile circularBuffer txb;	// Tx Buffer
volatile circularBuffer rxb;	// Rx Buffer

void comDrv_flushRx(void)
{
	//Initialize variables of the Rx Buffer
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
	rxb.overrun = 0;
	int i;
	for( i = 0; i < BUF_SIZE; i++)
	{
		rxb.data[i] = 0;
	}
}

void comDrv_flushTx(void)
{
	//Initialize variables of the Tx Buffer
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
	txb.overrun = 0;
	int i;
	for( i = 0; i < BUF_SIZE; i++)
	{
		txb.data[i] = 0;
	}
}

void comDrv_putc(char ch)
{
	while(txb.count == BUF_SIZE);	// Wait while Buffer is full
	txb.data[txb.tail] = ch;	// Copy character to the Tx Buffer at position 'tail'
	txb.tail = ( ++txb.tail) & INDEX_MASK;	// Increment 'tail' index ( mod. BUF_SIZE)
	DisableUart1TxInterrupt();	// Begin of critical section
	txb.count++;			// Increment 'count'
	EnableUart1TxInterrupt();		// End of critical section
}

void comDrv_puts(char *s)
{
	for( ; *s != '\0'; s++)
		comDrv_putc(*s);
}

char comDrv_getc(char *pchar)
{
	// Test 'count' variable (Rx Buffer) and return FALSE if zero
	if(rxb.count == 0)
		return 0;
	DisableUart1RxInterrupt();	// Begin of Critical Section
	*pchar = rxb.data[rxb.head];	// Copy 'head' character to *pchar
	rxb.count--;			// Decrement 'count'
	rxb.head = (rxb.head + 1) & INDEX_MASK;// Increment 'head' ( mod BUF_SIZE)
	EnableUart1RxInterrupt();	// End of Critical Section
	return 1;
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits)
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
	// Configure UART Interrupts
	//DisableUart1TxInterrupt();
	EnableUart1TxInterrupt();
	EnableUart1RxInterrupt();
	// Configurar prioridade
	IPC6bits.U1IP = 3;
	// Configuração do TX e RX para
	U1STAbits.UTXSEL = 00;		// ativar posição livre
	// ESTA PARTE ESTÁ MAL IMPLEMENTADA E NECESSITA DE REVISÃO
	// O UTXSEL DEVIA SER UTXISEL
	U1STAbits.URXISEL = 00;		// ativar leitura nova
	// Configurar TX e RX para ativação/inativação
	//IEC0bits.U1TXIE = 1;
	//IEC0bits.U1RXIE = 1;
	
	// Configuar deteção de erros framming, overrun, parity
	IEC0bits.U1EIE = 1;
}

int main(void)
{
	comDrv_config(115200,'N',1);	// Default 'pterm'
	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	comDrv_puts("PIC32 UART Device-Driver\n");
	char var;
	while(1)
	{	
		comDrv_getc(&var);
		comDrv_putc(var);
	}
	return 0;
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1TXIF)
	{
		if(txb.count > 0)
		{
			while(U1STAbits.UTXBF == 0 && txb.count)
			{
				// Copy character pointed by 'head' to U1TXREG
 				U1TXREG = txb.data[txb.head];
				// Increment 'head' variable (mod BUF_SIZE)
				txb.head = (txb.head + 1) & INDEX_MASK;
				// Decrement 'count' variable
				txb.count--;
			}
		}
		if(txb.count == 0)
			DisableUart1TxInterrupt();
		IFS0bits.U1TXIF = 0;
	}
	if(IFS0bits.U1RXIF)
	{	
		while(U1STAbits.URXDA && ( rxb.count < BUF_SIZE))
		{
			rxb.data[rxb.tail] = U1RXREG;	// Read character from UART and write
						// write to rxb.tail position
			// Increment 'tail' variable (mod BUF_SIZE)
			rxb.tail = (txb.head + 1) & INDEX_MASK;
			// If Rx Buffer is not full then increment 'count'
			if(rxb.count < BUF_SIZE)
			{
				rxb.count++;
				rxb.overrun = 0;
			}
			else
			{
				rxb.head++;
				rxb.overrun = 1;
				rxb.head = ( rxb.head + 1) & INDEX_MASK;
			}
		}
		IFS0bits.U1RXIF = 0;
	}
}
