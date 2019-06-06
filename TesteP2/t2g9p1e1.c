#include <detpic32.h>
#define DisableUart1RxInterrupt()	IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt()	IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt()	IEC0bits.U1TXIE	= 0
#define EnableUart1TxInterrupt()	IEC0bits.U1TXIE = 1
#define BUF_SIZE			32
#define INDEX_MASK			(BUF_SIZE - 1)

typedef struct
{
	unsigned char data[BUF_SIZE];
	unsigned int head;
	unsigned int tail;
	unsigned int count;
}circularBuffer;

volatile circularBuffer txb;
volatile circularBuffer rxb;

void comDrv_flushRx(void)
{
	int i;
	while(i < BUF_SIZE)
	{
		rxb.data[i] = 0;
		i++;
	}
	rxb.head = 0;
	rxb.tail = 0;
	rxb.count = 0;
}

void comDrv_flushTx(void)
{
	int i;
	while(i < BUF_SIZE)
	{
		txb.data[i] = 0;
		i++;
	}
	txb.head = 0;
	txb.tail = 0;
	txb.count = 0;
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

int main(void)
{
	return 0;
}
