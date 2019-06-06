void i2c1_init(unsigned int clock_freq)
{
	// Config BRG
	I2C1BRG = (20000000 + clock_freq)/(2 * clock_freq) - 1;
	// Enable I2C1
	I2C1CONbits.ON = 1;
}
