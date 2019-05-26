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
	for(; *s != '\0'; s++)
		comDrv_putc(*s);
}

void _int_(24) isr_uart1(void)
{
	/*
		Secção de serviço à interrupção de Transmissão
	*/
	if(IFS0bits.U1TXIF == 1)
	{
		if(txb.count > 0)
		{
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

int main(void)
{

	return 0;
}
