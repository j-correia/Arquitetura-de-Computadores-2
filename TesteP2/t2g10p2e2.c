#include "i2c.h"

int getTemperature(int *temperature)
{
	int ack;
	i2c1_init(TC74_CLK_FREQ);
	// Send Start Event
	i2c1_start();
	// Send Address + WR (ADD_WR); Copy return to ack
	ack = i2c1_send(ADD_WR);
	ack += i2c1_send(RTR);
	i2c1_start();
	ack += i2c1_send(ADD_RD);
	if(ack != 0)
	{
		i2c1_stop();
		printStr("Error.");
		exit(0);
	}
	*temperature = i2c1_receive(I2C_NACK);
	i2c1_stop();
	return ack;
}

int main(void)
{
	int temp;
	while(1)
	{
		getTemperature(&temp);
		printStr("Temperatura é: ");
		printInt10(temp);
		printStr("\r");
		delay(250);
	}
	return 0;
}
