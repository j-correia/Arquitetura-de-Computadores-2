// i2c.h
#ifndef __I2C_H_
#define __I2C_H_

// Declare symbols here (READ, WRITE, ...)
#define I2C_READ	1
#define I2C_WRITE	0
#define I2C_ACK		0
#define I2C_NACK	1
#define SENSE_ADDRESS	0x4D	// Device Dependent
#define ADDR_WR		((SENSE_ADDRESS << 1) | I2C_WRITE)
#define ADDR_RD		((SENSE_ADDRESS << 1) | I2C_READ)
#define TC74_CLK_FREQ	100000	// 100KHz
#define RTR		0	// Read Temperature Command

// Declare function prototypes here
void i2c1_init(unsigned int clock_freq);

void i2c1_start(void);

void i2c1_stop(void);

int i2c1_send(unsigned char value);

char i2c1_receive(char ack_bit);

#endif
