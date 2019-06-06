#include <detpic32.h>
#define I2C_READ	1
#define I2C_WRITE	0
#define I2C_ACK		0
#define I2C_NACK	1
#define SENS_ADDRESS	0x4D
#define ADD_WR		((SENS_ADDRESS << 1) | I2C_WRITE)
#define ADD_RD		((SENS_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ	100000
#define RTR		0

void i2c1_init(unsigned int clock_freq)
{
	// Config BRG
	I2C1BRG = (20000000 + clock_freq)/(2 * clock_freq) - 1;
	// Enable I2C1
	I2C1CONbits.ON = 1;
}

void i2c1_start(void)
{
	// wait until the lower 5bits of I2CxCON are all 0
	while((I2C1CON & 0x001F) != 0);
	// Activate start event
	I2C1CONbits.SEN = 1;
	while(I2C1CONbits.SEN == 1);
}

void i2c1_stop(void)
{
	// wait until lower 5bits of I2CxCON are 0
	while((I2C1CON & 0x001F) != 0);
	// Activate stop event
	I2C1CONbits.PEN = 1;
	// Wait for completion of stop event
	while(I2C1CONbits.PEN == 1);
}

int i2c1_send(unsigned char value)
{
	// Copy to I2C1TRN
	I2C1TRN = value;
	// Wait while Master Tx is in progress
	// ( 8 bits + ACK\) (I2C1STAT<TRSTAT>)
	while(I2C1STATbits.TRSTAT);
	return I2C1STATbits.ACKSTAT;
}

char i2c1_receive(char ack_bit)
{
	// Wait until lower 5 bits are all 0 in I2C1CON
	while((I2C1CON & 0x001F) != 0);
	// Activate I2C1CON<RCEN>
	I2C1CONbits.RCEN = 1;
	// Wait while byte not receive (I2C1STAT<RBF)
	while(I2C1STATbits.RBF == 0);
	// Send ACK/NACK
	if((ack_bit == 1) || !ack_bit)	
		I2C1CONbits.ACKDT = ack_bit;
	while((I2C1CON & 0x001F) != 0);
	// Start Ack sequence I2C1CON<ACKEN> = 1
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN == 1);
	return I2C1RCV;
}

void delay(int ms)
{
	while(ms >= 0)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
		ms--;
	}
}

int main(void)
{
	int ack, temperature;
	i2c1_init(TC74_CLK_FREQ);
	while(1)
	{
		// Send Start Event
		i2c1_start();
		// Send Address + WR (ADD_WR); Copy return to ack
		ack = i2c1_send(ADD_WR);
		ack += i2c1_send(RTR);
		i2c1_start();
		ack += i2c1_send(ADD_RD);
		if(ack != 0)
		{
			printStr("Error.");
			break;
		}
		temperature = i2c1_receive(I2C_NACK);
		i2c1_stop();
		printStr("Temperatura é: ");
		printInt10(temperature);
		printStr("\r");
		delay(250);
	}
	return 0;
}
