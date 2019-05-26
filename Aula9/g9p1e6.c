#include <detpic32.h>
#define	DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define	EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define	DisableUart1TxInterrupt()	IEC0bits.U1TXIE = 0
#define	EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1
#define BUF_SIZE 		  	8
#define INDEX_MASK 		  	(BUF_SIZE - 1)

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
	disableUart1TxInterrupt()	// Begins critical section
	txb.count++;			// Increment count variable
	enableUart1TxInterrupt();	// End of critical section
}

void comDrv_puts(char *s)
{
	while(*s != '\0')
	{
		comDrv_putc(s)
		s++;
	}
}

void _int_(24) isr_uart1(void)
{
	if(IFSbits.U1TXIF = 1)
	{
		if(txb.count > 0)
		{
			U1TXREG = txb.data[txb.head];		//Copy character pointed by "head" to U1TXREG register 
			txb.head = (txb.head + 1) & BUF_SIZE;	// Increment "head" variable (mod BUF_SIZE)
			txb.count--;		// decrement count
		}
		if(txb.count == 0)
		{
			disableUart1TxInterrupt();
		}
		IFSbits.U1TXIF = 0;
	}
}

