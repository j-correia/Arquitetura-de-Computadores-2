#include <detpic32.h>
#define	DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define	EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define	DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define	EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1
#define BUF_SIZE 		  	8
#define INDEX_MASK 		  	(BUF_SIZE - 1)
#define TRUE				1
#define FALSE				0

typedef struct
{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned tail;
	unsigned int count;
} circularBuffer;

volatile circularBuffer txb;	// Transmission Buffer
volatile circularBuffer rxb;	// Reception Buffer

void comDrv_flushRx(void)
{
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
	int i;
	for(i = 0; i < BUF_SIZE; i++)
	{
		rxb.data[i] = 0;
	}
}

void comDrv_flushTx(void)
{
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
	int i;
	for(i = 0; i < BUF_SIZE; i++)
	{
		txb.data[i] = 0;
	}
}

void comDrv_putc(char ch)
{
	while(txb.count == BUF_SIZE);	// Wait while Buffer full
	txb.data[txb.tail] = ch;	// Copy character to the txb @ pos. tail
	txb.tail = (txb.tail + 1) & INDEX_MASK;	// Increment tail mod. BUF_SIZE
	DisableUart1TxInterrupt();	// Begins critical section
	txb.count++;			// Increment count variable
	EnableUart1TxInterrupt();	// End of critical section
}

void comDrv_puts(char *s)
{
	/*while(*s != '\0')
	{
		comDrv_putc(*s);
		s++;
	}*/
	for(; *s != '\0'; s++)
		comDrv_putc(*s);
}

void _int_(24) isr_uart1(void)
{
	if(IFS0bits.U1TXIF = 1)
	{
		if(txb.count > 0)
		{
			/*U1TXREG = txb.data[txb.head];		//Copy character pointed by "head" to U1TXREG register 
			txb.head = (txb.head + 1) & BUF_SIZE;	// Increment "head" variable (mod BUF_SIZE)
			txb.count--;		// decrement count
			*/
			while(U1STAbits.UTXBF == 0 && txb.count != 0)
			{
				U1TXREG = txb.data[txb.head];
				txb.head = (txb.head + 1) & INDEX_MASK;
				txb.count--;
			}
		}
		if(txb.count == 0)
		{
			DisableUart1TxInterrupt();
		}
		IFS0bits.U1TXIF = 0;
	}
}

void comDrv_config(unsigned int baud, char parity, unsigned int stopbits)
{
	// Valores Default
	if(baud < 600 || baud > 115200)
	{
		baud = 115200;
	}
	if(parity != 'N' && parity != 'E' && parity != 'O')
	{
		parity = 'N';
	}
	if(stopbits != 1 && stopbits != 2)
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
	comDrv_config(115200, 'N', 1);	// Default pterm Parameters
	comDrv_flushRx();
	comDrv_flushTx();
	EnableInterrupts();
	while(1)
	{
		comDrv_puts("Teste do bloco de Transmissao do device driver!...");
	}	
	return 0;
}

