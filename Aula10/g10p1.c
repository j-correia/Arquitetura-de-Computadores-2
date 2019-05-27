#include <detpic32.h>
#define I2C_READ	1
#define I2C_WRITE	0
#define I2C_ACK		0
#define I2C_NACK	1
#define SENSE_ADDRESS	0x4D	// Device Dependent
#define ADDR_WR		((SENSE_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD		((SENSE_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ	100000	// 100KHz
#define RTR		0	// Read Temperature Command

void i2c1_init(unsigned int clock_freq)
{
	// Config baudrate generator (see intro for details)
	I2C1BRG = ( 20000000 + clock_freq) / (2*clock_freq) - 1;
	// Enable I2C1 module
	I2C1CONbits.ON = 1;
}

void i2c1_start(void)
{
	// Wait untill lower 5bits of I2CxCON all 0 lower 5 bits
	// of I2CxCON must be 0 before attempting to set SEN bit
	while((I2C1CON & 0x001F) != 0);
	// Activate Sart event (I2C1CON, bit SEN)
	I2C1CONbits.SEN = 1;
	// Wait for completion of the Start event (I2C1CON, bit SEN)
	while(I2C1CONbits.SEN != 1);
}

void i2c1_stop(void)
{
	// Wait until the lower 5bits of I2CxCON all 0 before
	// attempting to set PEN bit
	while((I2C1CON & 0x001F) != 0);
	// Activate stop event (I2C1CON, bit PEN)
	I2C1CONbits.PEN = 1;
	// Wait for completio of the Stop event (I2C1CON, bit PEN)
	while(I2C1CONbits.PEN != 1);
}

int i2c1_send(unsigned char value)
{
	// Copy "value" to I2C1TRN register
	I2C1TRN = value;
	// Wait while master Tx is in progress (8bits + ACK\)
	//	(I2C1STAT, bit TRSTAT - Tx status bit)
	while(I2C1STATbits.TRSTAT == 1);
	// Return acknowledge status bit (I2C1STAT, bit ACKSTAT)
	return I2C1STATbits.ACKSTAT;
}

char i2c1_receive(char ack_bit)
{
	// Wait until the lower 5bits of I2C1CON are 0 before
	// attempting to set the RCEN bit
	while((I2C1CON & 0x001F) != 0);
	// Activate RCEN bit (receive enable bit, I2C1CON register)
	I2C1CONbits.RCEN = 1;
	// Wait while byte not received (I2C1STAT, bit RBF - 
	// receive buffer full status bit)
	while(!I2C1STATbits.RBF);
	// Send ACK / NACK bit, for that:
	// 1. Copy "ack_bit" to I2C1CON, bit ACKDT ( be sure the 
	// "ack_bit" value is only 0 or 1);
	if(!ack_bit || ack_bit == 1)
	{
		I2C1CONbits.ACKDT = ack_bit;
	}
	// 2. Wait until the lower 5 bits of I2C1CON are all 0 
	// (the lower 5 bits of I2C1CON must be 0 before attempting
	// to set the ACKEN bit;
	while((I2C1CON & 0x001F) != 0);
	// 3. Start Acknowledge sequence (I2C1CON register, 
	// bit ACKEN = 1).
	I2C1CONbits.ACKEN = 1;
	// Wait for completion of Acknowledge sequence (I2C1CON,
	// bit ACKEN)
	while(I2C1CONbits.ACKEN == 0);
	// Return received value (I2C1RCV)
	return I2C1RCV;
}

void delay(int ms)
{
	for(; ms > 0; ms--)
	{
		resetCoreTimer();
		while(readCoreTimer() < 20000);
	}
}

int main(void)
{
	int ack, temperature;
	i2c1_init( TC74_CLK_FREQ);
	while(1)
	{
		// Send Start event
		i2c1_start();
		// Send Address + WR (ADDR_WR); copy return
		// value to "ack" variable
		ack = i2c1_send(ADDR_WR);
		// Send Command (RTR); add return value to 
		// "ack" variable
		ack += i2c1_send(RTR);
		// Send Start event (again)
		i2c1_start();
		// Send Address + RD (ADDR_RD); add return 
		// value to "ack" variable
		ack += i2c1_send(ADDR_RD);
		// Test "ack" variable; if "ack" != 0 then
		// an error has occurred; send the Stop event,
		// print an error message and exit loop
		if(ack != 0)
		{
			i2c1_stop();
			printStr("An error has occured.\n");
			break;
		}
		// Receive a value from slave (send NACK as
		// argument); copy
		// received value to "temperature" variable
		temperature = i2c1_send(I2C_NACK);
		// Send Stop event
		i2c1_stop();
		// Print "temperature" variable (syscall
		// printInt10)
		printInt10(temperature);
		// Wait 250 ms		
		delay(250);
	}
	return 0;	
}
